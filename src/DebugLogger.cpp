/**
 * @file DebugLogger.cpp
 * @brief Implementazione della classe DebugLogger.
 *
 * Questa classe gestisce i log del sistema, supportando sia la registrazione locale
 * che l'esposizione tramite un server HTTP. I log sono organizzati per ciclo di esecuzione
 * e livello di gravità.
 */

#include "DebugLogger.h"

DebugLogger *DebugLogger::instance = nullptr;

/**
 * @brief Costruttore privato per garantire il singleton.
 *
 * Inizializza il buffer per i log, il server HTTP e le variabili interne.
 */
DebugLogger::DebugLogger() : writeIndex(0), server(80)
{
    memset(buffer, 0, BUFFER_SIZE);
}

/**
 * @brief Ottiene l'istanza unica della classe DebugLogger.
 *
 * Se non esiste già un'istanza, la crea. Questo metodo garantisce che esista una
 * sola istanza della classe (pattern Singleton).
 *
 * @return Puntatore all'istanza di DebugLogger.
 */
DebugLogger *DebugLogger::getInstance()
{
    if (!instance)
    {
        instance = new DebugLogger();
    }
    return instance;
}

/**
 * @brief Imposta il livello minimo di log per la stampa su Serial.
 *
 * I messaggi con un livello inferiore a quello configurato non saranno stampati su Serial.
 *
 * @param level Livello minimo accettato per la stampa su Serial.
 */
void DebugLogger::setLogToSerialLevel(LogLevel level)
{
    logToSerialLevel = level;
}

/**
 * @brief Imposta il livello minimo di log per la memorizzazione nel buffer.
 *
 * I messaggi con un livello inferiore a quello configurato non saranno memorizzati nel buffer.
 *
 * @param level Livello minimo accettato per la memorizzazione nel buffer.
 */
void DebugLogger::setLogToBufferLevel(LogLevel level)
{
    logToBufferLevel = level;
}

/**
 * @brief Registra un messaggio di log.
 *
 * Salva il messaggio nel log organizzato per ciclo e livello. Può anche stampare il
 * messaggio su Serial se il livello è sufficiente.
 *
 * @param message Messaggio da registrare.
 * @param level Livello del messaggio (default: INFO).
 * @param printToSerial Se true, stampa il messaggio su Serial.
 */
void DebugLogger::log(const char *message, LogLevel level, bool printToSerial)
{
    // Aggiungi il messaggio alla struttura organizzata
    logData[cycleCount].logsByLevel[level].emplace_back(message);

    // Stampa su Serial se richiesto
    if (printToSerial && level >= logToSerialLevel)
    {
        Serial.print("[" + String(cycleCount) + "] " + message + "\n");
    }
}

/**
 * @brief Registra un valore numerico di log.
 *
 * Converte il valore numerico in una stringa e lo registra nel log organizzato per ciclo e livello.
 * Può anche stampare il valore su Serial.
 *
 * @param value Valore numerico da registrare.
 * @param level Livello del messaggio (default: INFO).
 * @param printToSerial Se true, stampa il valore su Serial.
 * @param decimalPlaces Numero di decimali da mantenere nel valore.
 */
void DebugLogger::log(double value, LogLevel level, bool printToSerial, unsigned int decimalPlaces)
{
    // Converte il valore in stringa
    String logMessage = String(value, decimalPlaces);

    // Aggiungi il messaggio alla struttura organizzata
    logData[cycleCount].logsByLevel[level].emplace_back(logMessage);

    // Stampa su Serial se richiesto
    if (printToSerial && level >= logToSerialLevel)
    {
        Serial.print("[" + String(cycleCount) + "] " + logMessage + "\n");
    }
}

/**
 * @brief Salva una riga formattata nel buffer.
 *
 * La riga formattata viene aggiunta al buffer principale, assicurando che non superi
 * la dimensione massima del buffer.
 *
 * @param formattedLog Stringa formattata da salvare.
 */
void DebugLogger::saveFormattedLog(const String &formattedLog)
{
    size_t messageLength = formattedLog.length() + 1; // Include '\0'
    if (messageLength > BUFFER_SIZE)
        return; // Scarta se il messaggio è troppo grande

    // Controlla se c'è spazio sufficiente
    if (writeIndex + messageLength >= BUFFER_SIZE)
    {
        writeIndex = 0; // Torna all'inizio (rotazione)
        // Indica buffer pieno
    }

    strcpy(&buffer[writeIndex], formattedLog.c_str());
    writeIndex += messageLength;
}

/**
 * @brief Stampa e salva tutti i log del ciclo corrente in formato standard.
 *
 * Per ogni livello di log del ciclo corrente, questo metodo genera una riga formattata
 * che include il numero del ciclo, il livello e i messaggi registrati.
 * Ogni riga viene stampata su Serial e salvata nel buffer principale.
 *
 * Dopo aver salvato i log nel buffer, i dati della struttura organizzata del ciclo corrente vengono liberati.
 */
void DebugLogger::printFormattedLogs()
{
    if (logData.find(cycleCount) != logData.end())
    {
        const LogCycle &cycleLogs = logData[cycleCount];

        for (const auto &[level, messages] : cycleLogs.logsByLevel)
        {
            // Formatta la riga del ciclo
            String formattedLog = "Cycle: " + String(cycleCount) + ", Level: " + String(static_cast<int>(level)) + ", Messages: ";
            for (const auto &message : messages)
            {
                formattedLog += message + " | ";
            }

            // Rimuove l'ultimo separatore
            if (!messages.empty())
            {
                formattedLog.remove(formattedLog.length() - 3); // Rimuove " | "
            }

            // Stampa su Serial e salva nel buffer
            Serial.println(formattedLog);
            saveFormattedLog(formattedLog);
        }

        // Libera la memoria della struttura organizzata per il ciclo corrente
        logData.erase(cycleCount);
    }
}

/**
 * @brief Formatta un livello di log in una stringa leggibile.
 */
std::string formatLogLevel(LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARNING:
        return "WARNING";
    case LogLevel::ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

/**
 * @brief Gestisce la richiesta per i log via HTTP.
 *
 * Risponde alla richiesta HTTP `/logs` inviando il contenuto del buffer di log.
 */
void DebugLogger::handleLogs()
{
    String level = server.arg("level");
    String cycle = server.arg("cycle");
    String range = server.arg("range");

    String response = "";

    if (!cycle.isEmpty())
    {
        size_t cycleNum = cycle.toInt();
        if (logData.find(cycleNum) != logData.end())
        {
            const auto &logsByLevel = logData.at(cycleNum).logsByLevel;
            for (auto it = logsByLevel.begin(); it != logsByLevel.end(); ++it)
            {
                response += String(formatLogLevel(it->first).c_str()) + ": ";
                for (const String &message : it->second)
                {
                    response += message + " | ";
                }
                response += "\n";
            }
        }
    }
    else if (!level.isEmpty())
    {
        LogLevel logLevel = static_cast<LogLevel>(level.toInt());
        for (const auto &[cycleNum, logCycle] : logData)
        {
            const auto &messages = logCycle.logsByLevel.at(logLevel);
            for (const String &message : messages)
            {
                response += "Cycle " + String(cycleNum) + ": " + message + "\n";
            }
        }
    }
    else
    {
        response = buffer; // Default behavior
    }

    server.send(200, "text/plain", response);
}

/**
 * @brief Avvia il server HTTP per servire i log.
 *
 * Configura il server HTTP per rispondere alla rotta `/logs` con il contenuto del buffer.
 */
void DebugLogger::startServer()
{
    server.on("/logs", [this]()
              { handleLogs(); });
    server.begin();
    Serial.println("Server HTTP avviato!");
}

/**
 * @brief Incrementa il contatore del ciclo di esecuzione.
 *
 * Questo metodo deve essere chiamato ad ogni iterazione del ciclo principale per aggiornare
 * il numero del ciclo corrente.
 */
void DebugLogger::incrementCycleCount()
{
    ++cycleCount;
}
