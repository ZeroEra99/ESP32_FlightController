#include "DebugLogger.h"

// Inizializzazione della variabile statica
DebugLogger* DebugLogger::instance = nullptr;

/**
 * @brief Costruttore privato per garantire il singleton.
 * 
 * @param serverUrl URL del server per la trasmissione dei log.
 */
DebugLogger::DebugLogger(const String& serverUrl) : writeIndex(0), serverUrl(serverUrl) {
    memset(buffer, 0, BUFFER_SIZE);
}

/**
 * @brief Distruttore della classe.
 */
DebugLogger::~DebugLogger() {
    delete instance;
}

/**
 * @brief Ottiene l'istanza unica della classe DebugLogger.
 * 
 * @param serverUrl URL del server per la trasmissione dei log (opzionale).
 * @return Puntatore alla singola istanza della classe.
 */
DebugLogger* DebugLogger::getInstance(const String& serverUrl) {
    if (!instance) {
        instance = new DebugLogger(serverUrl);
    }
    return instance;
}

/**
 * @brief Ottiene il timestamp corrente.
 * 
 * @return Stringa con il timestamp formattato (es. "[HH:MM:SS]").
 */
String DebugLogger::getTimestamp() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "[NO_TIME]";
    }
    char timeStr[10];
    strftime(timeStr, sizeof(timeStr), "[%H:%M:%S]", &timeinfo);
    return String(timeStr);
}

/**
 * @brief Aggiunge un messaggio al buffer di log, con la possibilità di stamparlo anche in seriale.
 * 
 * @param message Messaggio da registrare.
 * @param printToSerial Se true, stampa il messaggio anche in seriale.
 */
void DebugLogger::log(const char* message, bool printToSerial) {
    String timestampedMessage = getTimestamp() + " " + String(message) + "\n";
    if (printToSerial) {
        Serial.print(timestampedMessage);
    }
    size_t messageLength = timestampedMessage.length();
    if (writeIndex + messageLength < BUFFER_SIZE) {
        strcpy(&buffer[writeIndex], timestampedMessage.c_str());
        writeIndex += messageLength;
    } else {
        const char* overflowMessage = "[BUFFER OVERFLOW]\n";
        if (printToSerial) {
            Serial.print(overflowMessage);
        }
        strcpy(&buffer[writeIndex], overflowMessage);
        writeIndex += strlen(overflowMessage);
    }
}

/**
 * @brief Trasmette i log al server tramite HTTP.
 */
void DebugLogger::transmitLogs() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverUrl);
        http.addHeader("Content-Type", "text/plain");
        int httpResponseCode = http.POST(buffer);
        if (httpResponseCode > 0) {
            Serial.printf("Log inviati con successo: %d\n", httpResponseCode);
            memset(buffer, 0, BUFFER_SIZE);
            writeIndex = 0;
        } else {
            Serial.printf("Errore nell'invio dei log: %d\n", httpResponseCode);
        }
        http.end();
    } else {
        Serial.println("Connessione Wi-Fi non disponibile. Trasmissione fallita.");
    }
}
