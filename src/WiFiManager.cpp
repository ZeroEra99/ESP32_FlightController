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

    configTime(0, 0, "pool.ntp.org");
    while (time(nullptr) < 24 * 3600)
    {
        delay(100); // Aspetta che l'ora venga sincronizzata
    }

    // Avvia il mDNS
    if (!MDNS.begin("esp32"))
    {
        Logger::getInstance().log(LogLevel::ERROR, "Error starting mDNS.");
    }
    else
    {
        Logger::getInstance().log(LogLevel::INFO, "mDNS responder started.");
    }
}

void WiFiManager::discoverServer(const char *serverName)
{
    Serial.println("Searching for server.");
    int n = MDNS.queryService("http", "tcp"); // Cerca un servizio HTTP su TCP

    if (n == 0)
    {
        Serial.println("No service found.");
    }
    else
    {
        for (int i = 0; i < n; ++i)
        {
            // Confronta il nome del servizio con quello specificato
            if (MDNS.hostname(i) == serverName)
            {
                this->serverName = serverName;                           // Salva il nome del server
                IPAddress ip = MDNS.IP(i);                               // Ottieni l'indirizzo IP come IPAddress
                this->serverAddressString = ip.toString();               // Memorizza in std::string
                this->serverAddress = this->serverAddressString.c_str(); // Usa il puntatore dalla stringa persistente
                this->serverPort = MDNS.port(i);                         // Ottieni la porta

                Logger::getInstance().log(LogLevel::INFO, "Server found.");
                this->serverSet = true;
                return; // Fermati dopo aver trovato il server desiderato
            }
        }

        Serial.println("Target server not found.");
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

void WiFiManager::startServerCheckTask()
{
    xTaskCreatePinnedToCore(
        serverCheckTask,   // Funzione del task
        "ServerCheckTask", // Nome del task
        4096,              // Dimensione dello stack
        this,              // Parametro passato al task
        1,                 // Priorità
        nullptr,           // Handle del task
        1                  // Core su cui eseguire il task
    );
}

void WiFiManager::startServerDiscoveryTask(const char *serverName)
{
    struct TaskParams
    {
        WiFiManager *manager;
        const char *serverName;
    };

    TaskParams *params = new TaskParams{this, serverName}; // Alloca i parametri

    xTaskCreatePinnedToCore(
        serverDiscoveryTask,   // Funzione del task
        "ServerDiscoveryTask", // Nome del task
        4096,                  // Dimensione dello stack
        params,                // Parametro passato al task
        1,                     // Priorità
        nullptr,               // Handle del task
        1                      // Core su cui eseguire il task
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
            Logger::getInstance().log(LogLevel::WARNING, "WiFi not connected.");
            vTaskDelay(manager->checkInterval / portTICK_PERIOD_MS);
            continue;
        }
        String serverAddress = manager->serverAddress;
        if (!serverAddress.isEmpty() && manager->serverPort > 0)
        {
            // Prova a connettersi al server
            if (client.connect(manager->serverAddress, manager->serverPort))
            {
                client.stop();
                if (!manager->serverStatus)
                {
                    manager->serverStatus = true;
                    Logger::getInstance().log(LogLevel::INFO, "Connection to server established.");
                }
            }
            else
            {
                if (manager->serverStatus)
                {
                    manager->serverStatus = false;
                    Logger::getInstance().log(LogLevel::WARNING, "Connection to server failed.");
                }
            }
        }
        else
        {
            manager->serverStatus = false;
        }

        // Attesa fino al prossimo controllo
        vTaskDelay(manager->checkInterval / portTICK_PERIOD_MS);
    }
}

void WiFiManager::serverDiscoveryTask(void *param)
{
    struct TaskParams
    {
        WiFiManager *manager;
        const char *serverName;
    };

    TaskParams *params = static_cast<TaskParams *>(param);
    WiFiManager *manager = params->manager;
    const char *serverName = params->serverName;

    while (true)
    {
        if (!manager->serverSet)
        {
            manager->discoverServer(serverName);
        }
        vTaskDelay(manager->checkInterval / portTICK_PERIOD_MS);
    }

    delete params; // Libera i parametri alla fine (anche se il task non termina mai)
}
