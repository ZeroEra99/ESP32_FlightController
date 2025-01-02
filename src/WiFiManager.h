#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

/**
 * @class WiFiManager
 * @brief Gestione della connessione Wi-Fi per l'ESP32.
 */
class WiFiManager {
private:
    const char* ssid;         ///< SSID della rete Wi-Fi.
    const char* password;     ///< Password della rete Wi-Fi.
    bool connected;           ///< Stato della connessione.
    unsigned long lastReconnectAttempt; ///< Timestamp dell'ultimo tentativo di riconnessione.

    /**
     * @brief Tenta di connettersi al Wi-Fi in modo non bloccante.
     */
    void attemptConnection();

public:
    /**
     * @brief Costruttore della classe WiFiManager.
     * 
     * @param ssid SSID della rete Wi-Fi.
     * @param password Password della rete Wi-Fi.
     */
    WiFiManager(const char* ssid, const char* password);

    /**
     * @brief Inizializza la connessione Wi-Fi.
     */
    void connect();

    /**
     * @brief Disconnette la Wi-Fi.
     */
    void disconnect();

    /**
     * @brief Gestisce la connessione Wi-Fi in modo non bloccante.
     */
    void handleConnection();

    /**
     * @brief Verifica lo stato della connessione.
     * 
     * @return true se connesso, false altrimenti.
     */
    bool isConnected();

    /**
     * @brief Ottiene l'indirizzo IP del dispositivo.
     * 
     * @return Stringa con l'indirizzo IP.
     */
    String getLocalIP();
};

#endif // WIFI_MANAGER_H
