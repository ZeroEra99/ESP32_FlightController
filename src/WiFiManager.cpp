#include "WiFiManager.h"
//#include "DebugLogger.h"

WiFiManager::WiFiManager(const char *ssid, const char *password) : ssid(ssid), password(password)
{
    // Inizializza il WiFi Manager
    //DebugLogger::getInstance()->log("WiFiManager initialized.", LogLevel::DEBUG);
    Serial.println("WiFiManager initialized.");
}

void WiFiManager::manageAccessPoint(bool enable)
{
    if (enable)
    {
        // Configura l'Access Point con un IP statico
        IPAddress local_IP(192, 168, 1, 1);
        IPAddress gateway(192, 168, 1, 1);
        IPAddress subnet(255, 255, 255, 0);
        WiFi.softAPConfig(local_IP, gateway, subnet);

        // Avvia l'Access Point
        if (!WiFi.softAP(ssid, password))
        {
            //DebugLogger::getInstance()->log("Errore durante l'avvio dell'Access Point!", LogLevel::ERROR);
            return;
        }
        /*
        DebugLogger::getInstance()->log("Access Point avviato!", LogLevel::INFO);
        DebugLogger::getInstance()->log("Indirizzo IP: ", LogLevel::INFO);
        DebugLogger::getInstance()->log(WiFi.softAPIP());

        // Avvia il server HTTP per i log
        DebugLogger::getInstance()->startServer();
        DebugLogger::getInstance()->log("Server HTTP per i log avviato!", LogLevel::INFO);
        */
    }
    else
    {/*
        // Disabilita l'Access Point e il server HTTP
        DebugLogger::getInstance()->stopServer();
        DebugLogger::getInstance()->log("Server HTTP per i log disattivato!", LogLevel::INFO);*/
        WiFi.softAPdisconnect(true);
        //DebugLogger::getInstance()->log("Access Point disattivato!", LogLevel::INFO);
    }
}

String WiFiManager::getLocalIP()
{
    // Ritorna l'indirizzo IP locale come stringa
    return WiFi.softAPIP().toString();
}
