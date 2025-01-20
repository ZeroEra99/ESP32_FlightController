#include "Logger.h"
#include "WiFiManager.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

Logger::Logger() {}

// Singleton con oggetto statico locale
Logger &Logger::getInstance() {
    static Logger instance;
    return instance;
}

// Metodo per generare un log
void Logger::log(LogLevel level, const std::string &message)
{
    std::lock_guard<std::mutex> lock(mutex);

    // Formatta il log
    std::string formattedLog = formatLog(level, message);

    // Stampa il log sulla seriale (debug)
    Serial.println(formattedLog.c_str());

    // Verifica se il server è attivo prima di inviare il messaggio
    if (WiFiManager::getInstance().isServerActive()) {
        sendLogToServer(formattedLog);
    } else {
        Serial.println("Server not active. Unable to send log.");
    }
}

// Metodo per formattare un log
std::string Logger::formatLog(LogLevel level, const std::string &message) const
{
    std::string levelStr;
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

    unsigned long currentTime = millis();
    unsigned long seconds = (currentTime / 1000) % 60;
    unsigned long minutes = (currentTime / 60000) % 60;
    unsigned long hours = currentTime / 3600000;

    char timestamp[16];
    snprintf(timestamp, sizeof(timestamp), "%02lu:%02lu:%02lu", hours, minutes, seconds);

    return "[" + std::string(timestamp) + "] " + levelStr + ": " + message;
}

// Metodo per inviare un log al server
void Logger::sendLogToServer(const std::string &log)
{
    // Ottieni l'istanza del WiFiManager
    WiFiManager &wifiManager = WiFiManager::getInstance();

    // Ottieni l'indirizzo del server e la porta
    const char *serverAddress = wifiManager.serverAddress;
    uint16_t serverPort = wifiManager.serverPort;

    // Verifica che l'indirizzo e la porta siano validi
    if (!serverAddress || serverPort == 0) {
        Serial.println("Invalid server address or port. Unable to send log.");
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

        if (httpResponseCode <= 0) {
            Serial.println("Failed to send log to server. HTTP error: " + String(httpResponseCode));
        }

        http.end();
    }
    else
    {
        Serial.println("Wi-Fi not connected. Unable to send log.");
    }
}
