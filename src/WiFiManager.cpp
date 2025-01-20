#include "WiFiManager.h"
#include "Logger.h"
#include <WiFiClient.h>

WiFiManager &WiFiManager::getInstance()
{
    static WiFiManager instance;
    return instance;
}

void WiFiManager::begin(const char *ssid, const char *password)
{
    WiFi.begin(ssid, password);

    Logger::getInstance().log(LogLevel::INFO, "Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Logger::getInstance().log(LogLevel::INFO, "Connected to WiFi.");

    // Avvia mDNS per il client
    if (!MDNS.begin("esp32-client"))
    {
        Logger::getInstance().log(LogLevel::ERROR, "Failed to start mDNS.");
    }
    else
    {
        Logger::getInstance().log(LogLevel::INFO, "mDNS responder started.");
    }
}

bool WiFiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

bool WiFiManager::isServerActive()
{
    return serverStatus; // Restituisce lo stato aggiornato dal task
}

String WiFiManager::discoverServer()
{
    // Ricerca il server tramite mDNS
    Logger::getInstance().log(LogLevel::INFO, "Discovering server via mDNS...");
    IPAddress serverIP = MDNS.queryHost("ESP32Server");
    if (serverIP)
    {
        serverAddress = serverIP.toString();
        Logger::getInstance().log(LogLevel::INFO, "Server discovered.");
        return serverAddress;
    }
    else
    {
        Logger::getInstance().log(LogLevel::WARNING, "Server not found via mDNS.");
        return "";
    }
}

void WiFiManager::startServerCheckTask()
{
    xTaskCreatePinnedToCore(
        serverCheckTask,   // Funzione del task
        "ServerCheckTask", // Nome del task
        4096,              // Stack size
        this,              // Parametri passati al task
        1,                 // Priorità
        nullptr,           // Handle del task (non usato qui)
        1                  // Core su cui eseguire il task
    );
}

void WiFiManager::serverCheckTask(void *param)
{
    WiFiManager *manager = static_cast<WiFiManager *>(param);
    WiFiClient client;

    while (true)
    {
        if (!manager->isConnected())
        {
            manager->serverStatus = false;
            vTaskDelay(manager->checkInterval / portTICK_PERIOD_MS);
            continue;
        }

        // Se il server non è stato ancora scoperto, prova a scoprirlo
        if (manager->serverAddress.isEmpty())
        {
            manager->discoverServer();
        }

        // Prova a connettersi al server (se l'indirizzo è valido)
        if (!manager->serverAddress.isEmpty() && client.connect(manager->serverAddress.c_str(), manager->serverPort))
        {
            client.stop();
            if (manager->serverStatus != true)
            {
                manager->serverStatus = true;
                Logger::getInstance().log(LogLevel::INFO, "Connection to server established.");
            }
        }
        else
        {
            if (manager->serverStatus != false)
            {
                manager->serverStatus = false;
                Logger::getInstance().log(LogLevel::INFO, "Connection to server lost.");
            }
        }

        // Attende fino al prossimo controllo
        vTaskDelay(manager->checkInterval / portTICK_PERIOD_MS);
    }
}
