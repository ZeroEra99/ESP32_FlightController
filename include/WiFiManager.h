#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

class WiFiManager {
public:
    static WiFiManager &getInstance();

    void begin(const char *ssid, const char *password);
    bool isConnected();
    bool isServerActive();
    void startServerCheckTask(); // Avvia il task per il controllo del server

private:
    WiFiManager() = default;
    ~WiFiManager() = default;
    WiFiManager(const WiFiManager &) = delete;
    WiFiManager &operator=(const WiFiManager &) = delete;

    static void serverCheckTask(void *param); // Task FreeRTOS per il controllo del server

    bool serverStatus = false;
    const char *serverAddress = "192.168.1.2";
    uint16_t serverPort = 5000;
    unsigned long checkInterval = 200; // Intervallo tra i controlli in ms
};

#endif // WIFI_MANAGER_H
