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
        Serial.println("Server non raggiungibile, log non inviato.");
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
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        const char *serverAddress = "http://192.168.1.2:5000/receive";
        http.begin(serverAddress);
        http.addHeader("Content-Type", "text/plain");

        int httpResponseCode = http.POST(log.c_str());

        if (httpResponseCode > 0)
        {
        }
        else
        {
            Serial.println("Error sending log.");
            // Logger::getInstance()->log(LogLevel::ERROR, "Errore nell'invio del log."); Rompe tutto
        }

        http.end();
    }
    else
    {
        Serial.println("Wi-Fi not connected. Unable to send log.");
        // Logger::getInstance()->log(LogLevel::ERROR, "Wi-Fi non connesso. Impossibile inviare il log."); Rompe tutto
    }
}
