/**
 * @file WiFiManager.h
 * @brief Classe per gestire la creazione di un Access Point con l'ESP32.
 *
 * Questa classe consente di configurare e avviare un Access Point WiFi sull'ESP32,
 * fornendo un SSID e una password per l'accesso. Inoltre, permette di ottenere
 * l'indirizzo IP locale dell'AP.
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

/**
 * @class WiFiManager
 * @brief Gestisce l'Access Point per l'ESP32.
 *
 * La classe WiFiManager facilita la configurazione di una rete WiFi dedicata,
 * consentendo la comunicazione tra l'ESP32 e altri dispositivi tramite un indirizzo IP locale.
 */
class WiFiManager {
private:
    const char* ssid;         ///< SSID della rete AP.
    const char* password;     ///< Password della rete AP.

public:
    /**
     * @brief Costruttore della classe WiFiManager.
     *
     * Inizializza l'oggetto WiFiManager con i dettagli della rete AP.
     *
     * @param ssid SSID della rete AP.
     * @param password Password della rete AP.
     */
    WiFiManager(const char* ssid, const char* password);

    /**
     * @brief Avvia l'Access Point.
     *
     * Configura e avvia l'Access Point WiFi utilizzando l'SSID e la password forniti.
     */
    void startAccessPoint();

    /**
     * @brief Ottiene l'indirizzo IP dell'AP.
     *
     * Restituisce l'indirizzo IP assegnato all'Access Point sotto forma di stringa.
     *
     * @return Stringa con l'indirizzo IP dell'AP.
     */
    String getLocalIP();
};

#endif // WIFI_MANAGER_H
