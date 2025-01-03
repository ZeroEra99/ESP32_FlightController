/**
 * @file main.cpp
 * @brief Programma principale per il controllo di volo.
 *
 * Questo file contiene il ciclo principale del programma e la configurazione iniziale.
 * Include la configurazione dell'Access Point, l'avvio del server HTTP e l'integrazione con
 * il logger per il monitoraggio e la diagnostica.
 */

#include <Arduino.h>
#include "FlightController.h"

#include "WiFiManager.h"
#include "DebugLogger.h"

// Configurazione dell'Access Point
const char *ssid = "ESP32_AP";      ///< SSID per l'Access Point.
const char *password = "12345678"; ///< Password per l'Access Point.

// Dichiarazioni
WiFiManager wifiManager(ssid, password); ///< Gestore per l'Access Point WiFi.

/**
 * @brief Configurazione iniziale del sistema.
 *
 * Questa funzione configura la comunicazione seriale, avvia l'Access Point
 * e avvia il server HTTP per i log.
 */
void setup()
{
    Serial.begin(115200);

    // Avvia l'Access Point
    wifiManager.startAccessPoint();

    // Avvia il server HTTP
    DebugLogger::getInstance()->startServer();

    // Log iniziali
    DebugLogger::getInstance()->log("Flight controller started.", LogLevel::INFO);
}

/**
 * @brief Ciclo principale del programma.
 *
 * Il ciclo principale gestisce il logging dei dati formattati e monitora
 * lo stato del sistema tramite il server HTTP.
 */
void loop()
{
    // Log di debug
    DebugLogger::getInstance()->printFormattedLogs();
}
