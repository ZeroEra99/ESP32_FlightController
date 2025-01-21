/**
 * @file WiFiManager.h
 * @brief Dichiarazione della classe WiFiManager per la gestione della connessione WiFi.
 */
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <ESPmDNS.h>

/**
 * @brief Classe per la gestione della connessione WiFi.
 *
 * Questa classe fornisce metodi per la connessione al WiFi, la scoperta del server tramite mDNS
 * e il controllo dello stato della connessione.
 */
class WiFiManager
{
public:
    static WiFiManager &getInstance(); // Ottiene l'istanza del WiFiManager

    /**
     * @brief Inizializza la connessione WiFi.
     *
     * @param ssid SSID della rete WiFi.
     * @param password Password della rete WiFi.
     */
    void begin(const char *ssid, const char *password);

    /**
     * @brief Cerca un server mDNS.
     *
     * @param serverName Nome del server da cercare.
     */
    void discoverServer(const char *serverName);

    /**
     * @brief Verifica se la connessione WiFi è attiva.
     *
     * @return true se connesso, false altrimenti.
     */
    bool isConnected();

    /**
     * @brief Verifica se il server è attivo.
     *
     * @return true se il server è attivo, false altrimenti.
     */
    bool isServerActive();

    /**
     * @brief Avvia un task per il controllo dello stato del server.
     */
    void startServerCheckTask();

    /**
     * @brief Avvia un task per la scoperta del server mDNS.
     *
     * @param serverName Nome del server da cercare.
     */
    void startServerDiscoveryTask(const char *serverName);

    String serverAddressString;          ///< Indirizzo del server come stringa.
    const char *serverAddress = nullptr; ///< Indirizzo del server.
    const char *serverName = nullptr;    ///< Nome del server.
    uint16_t serverPort = 0;             ///< Porta del server.

private:
    /**
     * @brief Costruttore della classe WiFiManager.
     *
     * Il costruttore è privato per impedire la creazione di istanze esterne.
     * L'istanza può essere ottenuta tramite il metodo `getInstance()`.
     */
    WiFiManager() = default;

    /**
     * @brief Distruttore della classe WiFiManager.
     */
    ~WiFiManager() = default;

    WiFiManager(const WiFiManager &) = delete;            ///< Costruttore di copia disabilitato.
    WiFiManager &operator=(const WiFiManager &) = delete; ///< Operatore di assegnamento disabilitato.

    /**
     * @brief Task per il controllo dello stato del server.
     *
     * @param param Puntatore al WiFiManager.
     */
    static void serverCheckTask(void *param);

    /**
     * @brief Task per la scoperta del server mDNS.
     *
     * @param param Struttura con il puntatore al WiFiManager e il nome del server.
     */
    static void serverDiscoveryTask(void *param);

    bool serverSet = false;            ///< Flag per l'impostazione del server.
    bool serverStatus = false;         ///< Flag per lo stato del server.
    unsigned long checkInterval = 200; ///< Intervallo di controllo del server in millisecondi.
};

#endif // WIFI_MANAGER_H