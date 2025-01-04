/**
 * @file DebugLogger.h
 * @brief Classe per la gestione dei log e la loro esposizione tramite HTTP.
 *
 * Questa classe consente di registrare i log organizzandoli per ciclo di esecuzione e livello.
 * I log possono essere memorizzati localmente, stampati su Serial e resi disponibili tramite un server HTTP.
 */

#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include <map>
#include <vector>
#include <WiFi.h>
#include <WebServer.h>

/**
 * @enum LogLevel
 * @brief Livelli di log disponibili.
 *
 * I livelli di log determinano la gravità o il tipo di messaggio da registrare.
 */
enum class LogLevel
{
    DEBUG = 0,   ///< Messaggi di debug dettagliati.
    INFO = 1,    ///< Informazioni generali.
    WARNING = 2, ///< Avvisi non critici.
    ERROR = 3    ///< Errori critici.
};

/**
 * @struct LogCycle
 * @brief Struttura per contenere i messaggi di log separati per livello.
 *
 * I messaggi di log sono organizzati in base al loro livello di gravità
 * utilizzando una mappa che associa i livelli a vettori di stringhe.
 */
struct LogCycle
{
    std::map<LogLevel, std::vector<String>> logsByLevel; ///< Log separati per livello.
};

/**
 * @class DebugLogger
 * @brief Gestisce i log locali e li rende disponibili tramite un server HTTP.
 *
 * Questa classe utilizza un pattern Singleton per garantire un'unica istanza nel sistema.
 * I log sono organizzati per ciclo di esecuzione e livello di gravità, e possono essere:
 * - Stampati su Serial.
 * - Memorizzati in un buffer.
 * - Serviti tramite un server HTTP.
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
     * @brief Gestisce la richiesta per i log via HTTP.
     *
     * Questo metodo risponde alle richieste HTTP sulla rotta `/logs`
     * restituendo il contenuto del buffer di log.
     */
    void handleLogs();

    /**
     * @brief Costruttore privato per garantire il singleton.
     *
     * Inizializza il buffer dei log, il server HTTP e le variabili interne.
     */
    DebugLogger();

public:
    /**
     * @brief Ottiene l'istanza unica della classe DebugLogger.
     *
     * Garantisce che esista una sola istanza della classe (pattern Singleton).
     *
     * @return Puntatore all'istanza di DebugLogger.
     */
    static DebugLogger *getInstance();

    /**
     * @brief Imposta il livello minimo di log per la stampa su Serial.
     *
     * I messaggi con un livello inferiore a quello configurato non saranno stampati su Serial.
     *
     * @param level Livello minimo accettato per la stampa su Serial.
     */
    void setLogToSerialLevel(LogLevel level);

    /**
     * @brief Imposta il livello minimo di log per la memorizzazione nel buffer.
     *
     * I messaggi con un livello inferiore a quello configurato non saranno memorizzati nel buffer.
     *
     * @param level Livello minimo accettato per la memorizzazione nel buffer.
     */
    void setLogToBufferLevel(LogLevel level);

    /**
     * @brief Registra un messaggio di log.
     *
     * Il messaggio verrà registrato nel buffer e/o stampato su Serial
     * in base ai livelli configurati tramite `setLogToSerialLevel` e `setLogToBufferLevel`.
     *
     * @param message Messaggio da registrare.
     * @param level Livello del messaggio (default: INFO).
     * @param printToSerial Se true, stampa il messaggio su Serial.
     */
    void log(const char *message, LogLevel level = LogLevel::INFO, bool printToSerial = true);

    /**
     * @brief Registra un valore numerico di log.
     *
     * Il valore viene registrato nel buffer e/o stampato su Serial in base al livello configurato.
     *
     * @param value Valore numerico da registrare.
     * @param level Livello del messaggio (default: INFO).
     * @param printToSerial Se true, stampa il valore su Serial.
     * @param decimalPlaces Numero di decimali da mantenere nel valore (default: 1).
     */
    void log(double value, LogLevel level = LogLevel::INFO, bool printToSerial = true, unsigned int decimalPlaces = 1);

    /**
     * @brief Salva una riga formattata nel buffer.
     *
     * Questo metodo aggiunge una riga formattata, generata dai log del ciclo corrente, nel buffer principale.
     *
     * @param formattedLog Stringa formattata da salvare.
     */
    void saveFormattedLog(const String &formattedLog);

    /**
     * @brief Stampa e salva tutti i log del ciclo corrente in formato standard.
     *
     * Ogni riga rappresenta un livello di log per il ciclo corrente,
     * formattata con il numero del ciclo, il livello e i messaggi concatenati.
     */
    void printFormattedLogs();

    /**
     * @brief Avvia il server HTTP per servire i log.
     *
     * Configura il server HTTP per rispondere alle richieste sulla rotta `/logs`.
     */
    void startServer();

    /**
     * @brief Incrementa il contatore del ciclo di esecuzione.
     *
     * Questo metodo deve essere chiamato ad ogni iterazione del ciclo principale
     * per aggiornare il numero del ciclo corrente.
     */
    void incrementCycleCount();
};

#endif // DEBUG_LOGGER_H
