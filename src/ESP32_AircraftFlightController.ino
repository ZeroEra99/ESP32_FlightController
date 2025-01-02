#include <Arduino.h>
#include "FlightController.h"
#include "WiFiManager.h"
#include "DebugLogger.h"

/*
  Esempi di stampa e logging per il futuro
*/

// Credenziali Wi-Fi
const char *ssid = "NomeRete";
const char *password = "PasswordRete";
// URL del server per la trasmissione dei log
const String serverUrl = "http://192.168.1.100:8080/logs";

// Dichiarazioni
WiFiManager wifiManager(ssid, password);
DebugLogger *logger = DebugLogger::getInstance(serverUrl);

void setup()
{
    Serial.begin(115200);
    wifiManager.connect();
    DebugLogger::getInstance()->log("Inizializzazione in corso...");
}

void loop()
{
    // put your main code here, to run repeatedly:
    // Gestione Wi-Fi in base allo stato
    int currentState = 41;
    if (currentState == 2)
    {
        wifiManager.handleConnection(); // Logica non bloccante per la connessione
    }
    else
    {
        wifiManager.disconnect();
    }

    // Log periodico
    logger->log("Esempio di log.", currentState == 3);

    // Trasmissione dei log solo se connesso
    if (wifiManager.isConnected())
    {
        logger->transmitLogs();
    }
}
