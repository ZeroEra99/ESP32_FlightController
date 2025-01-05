/**
 * @file WiFiManager.h
 * @brief Dichiarazione della classe WiFiManager per la gestione dell'Access Point Wi-Fi.
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

/**
 * @brief Classe per la gestione del Wi-Fi e dell'Access Point.
 * 
 * Consente di configurare e gestire un Access Point (AP) per il monitoraggio remoto.
 */
class WiFiManager
{
private:
    const char *ssid;     ///< SSID della rete AP.
    const char *password; ///< Password della rete AP.

public:
    /**
     * @brief Costruttore della classe WiFiManager.
     * 
     * Inizializza l'Access Point con i parametri forniti.
     * 
     * @param ssid SSID della rete AP.
     * @param password Password della rete AP.
     */
    WiFiManager(const char *ssid, const char *password);

    /**
     * @brief Abilita o disabilita l'Access Point.
     * 
     * @param enable true per abilitare l'AP, false per disabilitarlo.
     */
    void manageAccessPoint(bool enable);

    /**
     * @brief Ottiene l'indirizzo IP locale dell'Access Point.
     * 
     * @return Indirizzo IP locale come stringa.
     */
    String getLocalIP();
};

#endif // WIFI_MANAGER_H
