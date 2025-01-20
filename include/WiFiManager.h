#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <ESPmDNS.h>

class WiFiManager
{
public:
    static WiFiManager &getInstance();

    void begin(const char *ssid, const char *password); // Connessione al WiFi
    void discoverServer(const char *serverName);        // Scoperta del server tramite mDNS
    bool isConnected();                                 // Verifica connessione WiFi
    bool isServerActive();                              // Stato del server
    void startServerCheckTask();                        // Avvia il task per controllare il server

    String serverAddressString;    // Indirizzo IP del server come stringa
    const char *serverAddress = nullptr; // Indirizzo IP del server
    const char *serverName = nullptr;    // Nome del server
    uint16_t serverPort = 0;             // Porta del server

private:
    WiFiManager() = default;
    ~WiFiManager() = default;
    WiFiManager(const WiFiManager &) = delete;
    WiFiManager &operator=(const WiFiManager &) = delete;

    static void serverCheckTask(void *param); // Task FreeRTOS per controllare il server

    bool serverStatus = false;         // Stato del server
    unsigned long checkInterval = 200; // Intervallo tra i controlli in ms
};

#endif // WIFI_MANAGER_H
