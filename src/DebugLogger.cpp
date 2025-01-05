#include "DebugLogger.h"

// Inizializzazione del singleton
DebugLogger *DebugLogger::instance = nullptr;

DebugLogger::DebugLogger() : writeIndex(0), server(80)
{
    memset(buffer, 0, BUFFER_SIZE);
}

DebugLogger *DebugLogger::getInstance()
{
    // Ritorna l'istanza singleton del logger
    if (!instance)
    {
        instance = new DebugLogger();
    }
    return instance;
}

void DebugLogger::setLogToSerialLevel(LogLevel level)
{
    // Imposta il livello minimo di log per Serial
    logToSerialLevel = level;
}

void DebugLogger::setLogToBufferLevel(LogLevel level)
{
    // Imposta il livello minimo di log per il buffer
    logToBufferLevel = level;
}

void DebugLogger::log(const char *message, LogLevel level, bool printToSerial)
{
    // Registra un messaggio di log
    logData[cycleCount].logsByLevel[level].emplace_back(message);

    // Stampa su Serial se richiesto
    if (printToSerial && level >= logToSerialLevel)
    {
        Serial.print("[" + String(cycleCount) + "] " + message + "\n");
    }
}

void DebugLogger::log(double value, LogLevel level, bool printToSerial, unsigned int decimalPlaces)
{
    // Converte il valore in stringa e registra il log
    String logMessage = String(value, decimalPlaces);
    logData[cycleCount].logsByLevel[level].emplace_back(logMessage);

    // Stampa su Serial se richiesto
    if (printToSerial && level >= logToSerialLevel)
    {
        Serial.print("[" + String(cycleCount) + "] " + logMessage + "\n");
    }
}

void DebugLogger::saveFormattedLog(const String &formattedLog)
{
    // Salva un log formattato nel buffer circolare
    size_t messageLength = formattedLog.length() + 1; // Include '\0'
    if (messageLength > BUFFER_SIZE)
        return; // Scarta se il messaggio è troppo grande

    if (writeIndex + messageLength >= BUFFER_SIZE)
    {
        writeIndex = 0; // Torna all'inizio (rotazione)
    }

    strcpy(&buffer[writeIndex], formattedLog.c_str());
    writeIndex += messageLength;
}

void DebugLogger::printFormattedLogs()
{
    // Stampa e libera i log del ciclo corrente
    if (logData.find(cycleCount) != logData.end())
    {
        const LogCycle &cycleLogs = logData[cycleCount];

        for (const auto &[level, messages] : cycleLogs.logsByLevel)
        {
            String formattedLog = "Cycle: " + String(cycleCount) + ", Level: " + String(static_cast<int>(level)) + ", Messages: ";
            for (const auto &message : messages)
            {
                formattedLog += message + " | ";
            }

            if (!messages.empty())
            {
                formattedLog.remove(formattedLog.length() - 3); // Rimuove " | "
            }

            Serial.println(formattedLog);
            saveFormattedLog(formattedLog);
        }

        logData.erase(cycleCount);
    }
}

void DebugLogger::handleLogs()
{
    // Gestisce le richieste HTTP per visualizzare i log
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
                response += String(static_cast<int>(it->first)) + ": ";
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
        response = buffer;
    }

    server.send(200, "text/plain", response);
}

void DebugLogger::startServer()
{
    // Avvia il server HTTP per i log
    server.on("/logs", [this]()
              { handleLogs(); });
    server.begin();
    Serial.println("HTTP server started!");
}

void DebugLogger::stopServer()
{
    // Ferma il server HTTP
    server.stop();
    Serial.println("HTTP server stopped!");
}

void DebugLogger::incrementCycleCount()
{
    // Incrementa il contatore dei cicli
    ++cycleCount;
}
