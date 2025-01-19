#include "WiFiManager.h"
#include <WiFiClient.h>

WiFiManager &WiFiManager::getInstance()
{
    static WiFiManager instance;
    return instance;
}

void WiFiManager::begin(const char *ssid, const char *password)
{
    WiFi.begin(ssid, password);
    Serial.print("Connessione al WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnesso al WiFi!");
}

bool WiFiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

bool WiFiManager::isServerActive()
{
    return serverStatus; // Restituisce lo stato aggiornato dal task
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

        // Prova a connettersi al server
        if (client.connect(manager->serverAddress, manager->serverPort))
        {
            client.stop();
            if (manager->serverStatus != true)
            {
                manager->serverStatus = true;
                Serial.println("Connessione con il server stabilita.");
            }
        }
        else
        {
            if (manager->serverStatus != false)
            {
                manager->serverStatus = false;
                Serial.println("Connessione con il server assente.");
            }
        }

        // Attende fino al prossimo controllo
        vTaskDelay(manager->checkInterval / portTICK_PERIOD_MS);
    }
}
