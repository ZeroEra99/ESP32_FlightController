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
    this->ssid = ssid;
    this->password = password;
}

bool WiFiManager::isServerActive()
{
    return serverStatus;
}

void WiFiManager::discoverServer(const char *serverName)
{
    static bool serviceFound = true, serverOk = true;
    int n = MDNS.queryService("http", "tcp"); // Cerca un servizio HTTP su TCP

    if (n == 0)
    {
        if (serviceFound)
        {
            Logger::getInstance().log(LogLevel::WARNING, "No service found.");
            serviceFound = false;
        }
    }
    else
    {
        if (!serviceFound)
        {
            Logger::getInstance().log(LogLevel::INFO, "Service found.");
            serviceFound = true;
        }
        for (int i = 0; i < n; ++i)
        {
            if (MDNS.hostname(i) == serverName)
            {
                this->serverName = serverName;
                IPAddress ip = MDNS.IP(i);
                this->serverAddressString = ip.toString();
                this->serverAddress = this->serverAddressString.c_str();
                this->serverPort = MDNS.port(i);

                if (!serverOk)
                {
                    Logger::getInstance().log(LogLevel::INFO, "Server found.");
                    serverOk = true;
                }
                this->serverSet = true;
                return;
            }
        }
        if (serverOk)
        {
            Logger::getInstance().log(LogLevel::WARNING, "Target server not found.");
            serverOk = false;
        }
    }
}

void WiFiManager::startConnectionTask()
{
    xTaskCreatePinnedToCore(
        WiFiConnectionTask,   // Funzione del task
        "WiFiConnectionTask", // Nome del task
        4096,                 // Dimensione dello stack
        this,                 // Parametro passato al task
        1,                    // Priorità
        nullptr,              // Handle del task
        1                     // Core su cui eseguire il task
    );
}

void WiFiManager::startServerDiscoveryTask(const char *serverName)
{
    struct TaskParams
    {
        WiFiManager *manager;
        const char *serverName;
    };

    TaskParams *params = new TaskParams{this, serverName};

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

void WiFiManager::WiFiConnectionTask(void *param)
{
    WiFiManager *manager = static_cast<WiFiManager *>(param);

    while (true)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            if (manager->wifiConnected)
                Logger::getInstance().log(LogLevel::WARNING, "WiFi connection lost.");

            manager->wifiConnected = false;

            Logger::getInstance().log(LogLevel::WARNING, "Attempting to connect to WiFi...");
            WiFi.begin(manager->ssid, manager->password);
            while (WiFi.status() != WL_CONNECTED)
                ;
            // Configura il time NTP
            Logger::getInstance().log(LogLevel::INFO, "Synchronizing time with NTP server...");
            configTime(0, 0, "pool.ntp.org");
            while (time(nullptr) < 24 * 3600)
            {
                delay(100); // Aspetta che l'ora venga sincronizzata
            }
            Logger::getInstance().log(LogLevel::INFO, "Time synchronized.");

            // Avvia il mDNS
            if (!MDNS.begin("esp32"))
            {
                Logger::getInstance().log(LogLevel::ERROR, "Error starting mDNS.");
            }
            else
            {
                Logger::getInstance().log(LogLevel::INFO, "mDNS responder started.");
            }

            manager->wifiConnected = true;
            Logger::getInstance().log(LogLevel::INFO, "Connected to WiFi.");
        }
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
        if (!manager->serverSet && manager->wifiConnected)
        {
            manager->discoverServer(serverName);
        }
        else if (!manager->wifiConnected)
        {
            manager->serverSet = false;
        }
        vTaskDelay(manager->checkServerInterval / portTICK_PERIOD_MS);
    }

    delete params;
}

void WiFiManager::serverCheckTask(void *param)
{
    WiFiManager *manager = static_cast<WiFiManager *>(param);
    WiFiClient client;

    while (true)
    {
        if (!manager->wifiConnected || !manager->serverSet)
        {
            if (manager->serverStatus)
            {
                manager->serverStatus = false;
                Logger::getInstance().log(LogLevel::WARNING, "WiFi not connected. Skipping server check.");
            }
        }
        else
        {
            String serverAddress = manager->serverAddress;
            if (!serverAddress.isEmpty() && manager->serverPort > 0)
            {
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
        }
        vTaskDelay(manager->checkServerInterval / portTICK_PERIOD_MS);
    }
}