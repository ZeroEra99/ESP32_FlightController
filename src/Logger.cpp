#include "Logger.h"
#include "WiFiManager.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

Logger::Logger()
{
    // Costruttore vuoto
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

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

void Logger::log(LogLevel level, const std::string &message, bool sendToServer)
{
    static bool bufferFull = false;
    std::lock_guard<std::mutex> lock(mutex);
    std::string formattedLog = formatLog(level, message);
    Serial.println(formattedLog.c_str());
    if (!sendToServer)
        return;
    if (logBuffer.size() >= maxBufferSize)
    {
        logBuffer.pop_front();
        if (!bufferFull)
        {
            Logger::getInstance().log(LogLevel::WARNING, "LogBuffer is full. Older logs will be discarded.");
            bufferFull = true;
        }
    }
    else
    {
        if (bufferFull)
        {
            Logger::getInstance().log(LogLevel::WARNING, "LogBuffer is no longer full.");
            bufferFull = false;
        }
    }
    logBuffer.push_back(formattedLog);
}

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

void Logger::sendLogToServer(const std::string &log)
{
    WiFiManager &wifiManager = WiFiManager::getInstance();
    const char *serverAddress = wifiManager.serverAddress;
    uint16_t serverPort = wifiManager.serverPort;

    if (!serverAddress || serverPort == 0)
        return;

    String serverUrl = String("http://") + serverAddress + ":" + String(serverPort) + "/receive";

    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        http.begin(serverUrl.c_str());
        http.addHeader("Content-Type", "text/plain");

        int httpResponseCode = http.POST(log.c_str());

        if (httpResponseCode <= 0)
        {
            Serial.println("Failed to send log to server. HTTP error: " + String(httpResponseCode));
        }

        http.end();
    }
}

void Logger::incrementCycle()
{
    std::lock_guard<std::mutex> lock(mutex);
    currentCycle++;
    tempDataRow.clear();
    tempDataRow.push_back(std::to_string(currentCycle));
}

void Logger::logData(const std::string &varName, double value)
{
    std::lock_guard<std::mutex> lock(mutex);
    tempDataRow.push_back(std::to_string(value));
}

void Logger::prepareDataBuffer()
{
    std::lock_guard<std::mutex> lock(mutex);
    static bool bufferFull = true;
    if (tempDataRow.empty())
    {
        Logger::getInstance().log(LogLevel::ERROR, "No data to log.");
        return;
    }

    if (dataBuffer.size() >= maxBufferSize)
    {
        dataBuffer.pop_front();
        if (!bufferFull)
        {
            Logger::getInstance().log(LogLevel::WARNING, "DataBuffer is full. Older data will be discarded.");
            bufferFull = true;
        }
    }
    else
    {
        if (bufferFull)
        {
            Logger::getInstance().log(LogLevel::WARNING, "DataBuffer is no longer full.");
            bufferFull = false;
        }
    }

    dataBuffer.push_back(tempDataRow);
}

void Logger::sendDataToServer()
{
    WiFiManager &wifiManager = WiFiManager::getInstance();
    const char *serverAddress = wifiManager.serverAddress;
    uint16_t serverPort = wifiManager.serverPort;

    if (!serverAddress || serverPort == 0)
        return;

    String serverUrl = String("http://") + serverAddress + ":" + String(serverPort) + "/get_data_logs";

    while (!dataBuffer.empty() && WiFiManager::getInstance().isServerActive())
    {
        std::vector<std::string> row;

        {
            std::lock_guard<std::mutex> lock(mutex);
            row = dataBuffer.front();
        }

        String jsonData = "[";
        for (size_t i = 0; i < row.size(); ++i)
        {
            jsonData += "\"" + String(row[i].c_str()) + "\"";
            if (i < row.size() - 1)
                jsonData += ",";
        }
        jsonData += "]";

        HTTPClient http;
        http.begin(serverUrl.c_str());
        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST(jsonData);

        if (httpResponseCode > 0)
        {
            std::lock_guard<std::mutex> lock(mutex);
            dataBuffer.pop_front();
        }
        else
        {
            String response = "Failed to send data logs to server. HTTP error: " + String(httpResponseCode);
            Logger::getInstance().log(LogLevel::ERROR, response.c_str());
            break;
        }

        http.end();
    }
}

void Logger::printCurrentCycleData() const
{
    std::lock_guard<std::mutex> lock(const_cast<std::mutex &>(mutex));
    for (const auto &cell : tempDataRow)
    {
        Serial.print(cell.c_str());
        Serial.print("\t");
    }
    Serial.println();
}

void Logger::logTask(void *param)
{
    Logger *logger = static_cast<Logger *>(param);

    while (true)
    {
        if (WiFiManager::getInstance().isServerActive())
        {
            // Invio dei log
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

            // Invio dei dati
            logger->sendDataToServer();
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
