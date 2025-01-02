#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include <WiFi.h>
#include <HTTPClient.h>

/**
 * @class DebugLogger
 * @brief Classe per gestire il logging locale e la trasmissione dei log via HTTP.
 */
class DebugLogger {
private:
    static DebugLogger* instance;              ///< Puntatore alla singola istanza della classe.
    static constexpr size_t BUFFER_SIZE = 2048; ///< Dimensione del buffer circolare.
    char buffer[BUFFER_SIZE];                   ///< Buffer per i log.
    size_t writeIndex;                          ///< Indice di scrittura.
    String serverUrl;                           ///< URL del server per la trasmissione.

    /**
     * @brief Costruttore privato per garantire il singleton.
     * 
     * @param serverUrl URL del server per la trasmissione dei log.
     */
    DebugLogger(const String& serverUrl);

    /**
     * @brief Ottiene il timestamp corrente.
     * 
     * @return Stringa con il timestamp formattato.
     */
    String getTimestamp();

public:
    /**
     * @brief Distruttore della classe.
     */
    ~DebugLogger();

    /**
     * @brief Ottiene l'istanza unica della classe DebugLogger.
     * 
     * @param serverUrl URL del server per la trasmissione dei log (opzionale).
     * @return Puntatore alla singola istanza della classe.
     */
    static DebugLogger* getInstance(const String& serverUrl = "");

    /**
     * @brief Aggiunge un messaggio al buffer di log, con la possibilità di stamparlo anche in seriale.
     * 
     * @param message Messaggio da registrare.
     * @param printToSerial (Opzionale) Se true, stampa il messaggio anche in seriale. Default: false.
     */
    void log(const char* message, bool printToSerial = false);

    /**
     * @brief Trasmette i log al server tramite HTTP.
     */
    void transmitLogs();
};

#endif // DEBUG_LOGGER_H
