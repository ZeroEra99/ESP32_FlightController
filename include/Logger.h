/**
 * @file Logger.h
 * @brief Dichiarazione della classe Logger per la gestione dei log di sistema.
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>
#include <mutex>

/**
 * @brief Enumerazione per i livelli di log disponibili.
 */
enum class LogLevel
{
    ERROR,   ///< Errore critico.
    WARNING, ///< Avvertimento.
    INFO,    ///< Informazione.
};

/**
 * @brief Classe per la gestione dei log di sistema.
 *
 * Fornisce un'interfaccia per la registrazione e l'invio dei log di sistema.
 * I log vengono formattati in modo standard e inviati a un server remoto.
 *
 * La classe è implementata come un Singleton per garantire un'unica istanza globale.
 */
class Logger
{
private:
    /**
     * @brief Costruttore privato della classe Logger.
     *
     * Il costruttore è privato per impedire la creazione di istanze esterne.
     */
    Logger();

    std::mutex mutex;             ///< Mutex per la sincronizzazione.

public:
    /**
     * @brief Ottiene l'istanza del logger.
     *
     * @return Puntatore all'istanza del logger.
     */
    static Logger &getInstance();

    /**
     * @brief Registra un messaggio di log.
     *
     * @param level Livello di log.
     * @param message Messaggio da registrare.
     */
    void log(LogLevel level, const std::string &message);

    /**
     * @brief Formatta un messaggio di log.
     *
     * @param level Livello di log.
     * @param message Messaggio da formattare.
     *
     * @return Messaggio formattato.
     */
    std::string formatLog(LogLevel level, const std::string &message) const;

    /**
     * @brief Invia un log al server remoto.
     *
     * @param log Messaggio di log da inviare.
     */
    void sendLogToServer(const std::string &log);

    ~Logger() = default; ///< Distruttore di default.
};

#endif // LOGGER_H
