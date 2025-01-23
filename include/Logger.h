/**
 * @file Logger.h
 * @brief Dichiarazione della classe Logger per la gestione dei log di sistema e dei dati numerici.
 */
#ifndef LOGGER_H
#define LOGGER_H

#include "DataStructures.h"
#include <string>
#include <vector>
#include <mutex>
#include <deque>
#include <iostream>

/**
 * @brief Classe per la gestione dei log di sistema e dei dati numerici.
 *
 * La classe fornisce un'interfaccia per la registrazione e l'invio di log di sistema
 * e dati numerici. Implementata come Singleton.
 */
class Logger
{
private:
    Logger(); ///< Costruttore privato per garantire il Singleton.

    std::mutex mutex;                  ///< Mutex per la sincronizzazione.
    std::deque<std::string> logBuffer; ///< Buffer circolare per i log.
    size_t maxBufferSize = 100;        ///< Dimensione massima del buffer dei log.

    std::deque<std::vector<std::string>> dataBuffer; ///< Buffer per i dati numerici.
    size_t maxDataBufferSize = 2000;                  ///< Dimensione massima del buffer dei dati.
    std::vector<std::string> tempDataRow;            ///< Riga temporanea per i dati del ciclo corrente.
    size_t currentCycle = 0;                         ///< Contatore del ciclo corrente.
    std::string startTimestamp;                      ///< Timestamp di inizio raccolta dati.

    bool headerInitialized = false; ///< Indica se l'header è stato inizializzato

    static void logTask(void *param); ///< Task FreeRTOS per l'invio asincrono dei log.

public:
    static Logger &getInstance(); ///< Ottiene l'istanza Singleton.

    void startLogTask(); ///< Avvia il task di invio asincrono dei log.

    void log(LogLevel level, const std::string &message, bool sendToServer = true); ///< Registra un messaggio di log.

    std::string formatLog(LogLevel level, const std::string &message) const; ///< Formatta un messaggio di log.

    void sendLogToServer(const std::string &log); ///< Invia un log al server remoto.

    void incrementCycle(); ///< Incrementa il contatore del ciclo e prepara la riga dati.

    void logData(const std::string &varName, double value, int decimalPlaces = 3); ///< Registra un dato numerico.

    void prepareDataBuffer(); ///< Organizza i dati raccolti nel buffer principale.

    void sendDataToServer(); ///< Invia i dati raccolti al server remoto.

    void printCurrentCycleData() const; ///< Stampa i dati del ciclo corrente sulla seriale.

    ~Logger() = default; ///< Distruttore di default.
};

#endif // LOGGER_H
