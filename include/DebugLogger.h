/**
 * @file DebugLogger.h
 * @brief Dichiarazione della classe DebugLogger per la gestione del logging e del server HTTP.
 */

#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include <map>
#include <vector>
#include <WiFi.h>
#include <WebServer.h>

/**
 * @brief Livelli di log supportati.
 * 
 * Ogni livello rappresenta una categoria di messaggi con priorità differente.
 */
enum class LogLevel
{
    DEBUG = 0,     ///< Messaggi di debug dettagliati.
    ERROR = 1,     ///< Errori critici.
    WARNING = 2,   ///< Avvisi non critici.
    INFO = 3,      ///< Informazioni generali.
    DATA = 4,      ///< Dati relativi al sistema.
    DATA_EXTRA = 5 ///< Dati extra relativi al sistema.
};

/**
 * @brief Struttura per memorizzare i log organizzati per ciclo e livello.
 */
struct LogCycle
{
    std::map<LogLevel, std::vector<String>> logsByLevel; ///< Log separati per livello.
};

/**
 * @brief Classe per la gestione e il monitoraggio dei log di sistema.
 * 
 * Implementa un singleton per la registrazione dei log e offre un server HTTP per l'accesso remoto.
 */
class DebugLogger
{
private:
    static DebugLogger *instance;                 ///< Singleton della classe.
    static constexpr size_t BUFFER_SIZE = 153600; ///< Dimensione del buffer per i log.
    char buffer[BUFFER_SIZE];                     ///< Buffer per memorizzare i log formattati.
    size_t writeIndex;                            ///< Indice di scrittura del buffer.
    WebServer server;                             ///< Server HTTP per servire i log.

    LogLevel logToSerialLevel = LogLevel::INFO;  ///< Livello minimo di log per la stampa su Serial.
    LogLevel logToBufferLevel = LogLevel::DEBUG; ///< Livello minimo di log per la memorizzazione nel buffer.

    size_t cycleCount = 0;              ///< Contatore del numero del ciclo di esecuzione.
    std::map<size_t, LogCycle> logData; ///< Log organizzati per ciclo e livello.

    /**
     * @brief Gestisce le richieste HTTP per visualizzare i log.
     */
    void handleLogs();

    /**
     * @brief Costruttore privato per il singleton.
     */
    DebugLogger();

public:
    /**
     * @brief Ottiene l'istanza singleton del logger.
     * 
     * @return Puntatore all'istanza di DebugLogger.
     */
    static DebugLogger *getInstance();

    /**
     * @brief Imposta il livello minimo di log per la stampa su Serial.
     * 
     * @param level Livello minimo di log.
     */
    void setLogToSerialLevel(LogLevel level);

    /**
     * @brief Imposta il livello minimo di log per la memorizzazione nel buffer.
     * 
     * @param level Livello minimo di log.
     */
    void setLogToBufferLevel(LogLevel level);

    /**
     * @brief Registra un messaggio di log.
     * 
     * @param message Messaggio da registrare.
     * @param level Livello del messaggio.
     * @param printToSerial true per stampare il messaggio su Serial, false altrimenti.
     */
    void log(const char *message, LogLevel level = LogLevel::INFO, bool printToSerial = true);

    /**
     * @brief Registra un valore numerico come log.
     * 
     * @param value Valore numerico da registrare.
     * @param level Livello del messaggio.
     * @param printToSerial true per stampare il valore su Serial, false altrimenti.
     * @param decimalPlaces Numero di cifre decimali da includere.
     */
    void log(double value, LogLevel level = LogLevel::INFO, bool printToSerial = true, unsigned int decimalPlaces = 1);

    /**
     * @brief Salva un log formattato nel buffer.
     * 
     * @param formattedLog Stringa del log formattato.
     */
    void saveFormattedLog(const String &formattedLog);

    /**
     * @brief Stampa i log formattati su Serial.
     */
    void printFormattedLogs();

    /**
     * @brief Avvia il server HTTP per la visualizzazione remota dei log.
     */
    void startServer();

    /**
     * @brief Ferma il server HTTP.
     */
    void stopServer();

    /**
     * @brief Incrementa il contatore dei cicli di log.
     */
    void incrementCycleCount();
};

#endif // DEBUG_LOGGER_H
