#include "Logger.h"
#include "WiFiManager.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

Logger::Logger() {}

void Logger::startLogTask()
{
    xTaskCreatePinnedToCore(
        logTask,   // Funzione del task
        "LogTask", // Nome del task
        4096,      // Dimensione dello stack
        this,      // Parametro passato al task
        1,         // Priorità del task
        nullptr,   // Handle del task
        1          // Core su cui eseguire il task
    );
}

// Singleton con oggetto statico locale
Logger &Logger::getInstance()
{
    static Logger instance; ///< Istanza del logger
    return instance;
}

// Metodo per generare un log
void Logger::log(LogLevel level, const std::string &message)
{
    std::lock_guard<std::mutex> lock(mutex); // Blocca il mutex per evitare accessi concorrenti

    std::string formattedLog = formatLog(level, message); // Formatta il log

    Serial.print(formattedLog.c_str()); // Stampa il log sulla seriale

    // Se il buffer è pieno, scarta il messaggio più vecchio
    if (logBuffer.size() >= maxBufferSize)
    {
        logBuffer.pop_front();
        Serial.print("\tLog buffer full. Oldest log discarded.");
    }
    // Aggiungi il log al buffer
    logBuffer.push_back(formattedLog);
    Serial.println("\tLog queued.");
}

std::string Logger::formatLog(LogLevel level, const std::string &message) const
{
    std::string levelStr; // Stringa per il livello di log
    switch (level)
    {
    case LogLevel::ERROR:
        levelStr = "ERROR";
        break;
    case LogLevel::WARNING:
        levelStr = "WARNING";
        break;
    case LogLevel::INFO:
        levelStr = "INFO";
        break;
    }

    // Ottieni il timestamp attuale
    unsigned long currentTime = millis();
    unsigned long seconds = (currentTime / 1000) % 60;
    unsigned long minutes = (currentTime / 60000) % 60;
    unsigned long hours = currentTime / 3600000;

    char timestamp[16];
    snprintf(timestamp, sizeof(timestamp), "%02lu:%02lu:%02lu", hours, minutes, seconds); // Formatta il timestamp

    return "[" + std::string(timestamp) + "] " + levelStr + ": " + message;
}

void Logger::sendLogToServer(const std::string &log)
{
    // Ottieni l'istanza del WiFiManager
    WiFiManager &wifiManager = WiFiManager::getInstance();

    // Ottieni l'indirizzo del server e la porta
    const char *serverAddress = wifiManager.serverAddress;
    uint16_t serverPort = wifiManager.serverPort;

    // Verifica che l'indirizzo e la porta siano validi
    if (!serverAddress || serverPort == 0)
    {
        return;
    }

    // Costruisci l'URL del server
    String serverUrl = String("http://") + serverAddress + ":" + String(serverPort) + "/receive";

    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        // Inizia la connessione HTTP
        http.begin(serverUrl.c_str());
        http.addHeader("Content-Type", "text/plain");

        // Esegui la richiesta POST con il log
        int httpResponseCode = http.POST(log.c_str());

        if (httpResponseCode <= 0)
        {
            Serial.println("Failed to send log to server. HTTP error: " + String(httpResponseCode));
        }

        http.end();
    }
}

// Metodo per impostare la dimensione massima del buffer
void Logger::setMaxBufferSize(size_t size)
{
    std::lock_guard<std::mutex> lock(mutex);
    maxBufferSize = size;

    // Se la nuova dimensione è inferiore al numero di messaggi in coda, rimuovi gli elementi più vecchi
    while (logBuffer.size() > maxBufferSize)
    {
        logBuffer.pop_front();
    }
}

void Logger::logTask(void *param)
{
    Logger *logger = static_cast<Logger *>(param);

    while (true)
    {
        if (WiFiManager::getInstance().isServerActive())
        {
            std::string logMessage;

            {
                std::lock_guard<std::mutex> lock(logger->mutex);
                if (!logger->logBuffer.empty())
                {
                    logMessage = logger->logBuffer.front();
                    logger->logBuffer.pop_front();
                }
            }

            if (!logMessage.empty())
            {
                logger->sendLogToServer(logMessage);
            }
        }

        // Aspetta brevemente prima di riprovare, sia che il server sia attivo o meno
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
