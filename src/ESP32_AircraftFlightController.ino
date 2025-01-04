/**
 * @file main.cpp
 * @brief Programma principale per il controllo di volo.
 *
 * Questo file contiene il ciclo principale del programma e la configurazione iniziale.
 * Si occupa della configurazione dell'Access Point WiFi, dell'avvio del server HTTP
 * per il monitoraggio e la diagnostica, e dell'integrazione con il sistema di logging.
 */

#include <Arduino.h>
#include "SystemController.h"
#include "WiFiManager.h"
#include "DebugLogger.h"

// Configurazione dell'Access Point
const char *ssid = "ESP32_AP";     ///< Nome della rete WiFi (SSID) per l'Access Point.
const char *password = "12345678"; ///< Password della rete WiFi per l'accesso all'Access Point.

// Dichiarazioni
WiFiManager wifiManager(ssid, password);          ///< Gestore dell'Access Point WiFi.
SystemController systemController;                ///< Controller principale del sistema di volo.
DebugLogger *logger = DebugLogger::getInstance(); ///< Logger per i messaggi di log.

static unsigned long tPrev = 0; ///< Timestamp dell'ultimo ciclo in millisecondi.

/**
 * @brief Configurazione iniziale del sistema.
 *
 * In questa funzione viene configurata la comunicazione seriale per il debug,
 * avviato l'Access Point WiFi per la connessione al sistema e configurato il server
 * HTTP per il monitoraggio remoto. Inoltre, vengono registrati log iniziali per
 * confermare l'avvio corretto del sistema.
 */
void setup()
{
    Serial.begin(115200); // Configurazione della porta seriale a 115200 baud

    // Avvio dell'Access Point WiFi
    wifiManager.startAccessPoint();

    // Configurazione del server HTTP per il logging
    logger->startServer();

    // Log iniziale per indicare l'avvio del sistema
    logger->log("Flight controller started.", LogLevel::INFO);
}

/**
 * @brief Ciclo principale del programma.
 *
 * Il ciclo principale si occupa di:
 * - Lettura dei dati dai sensori (IMU e ricevitore RC).
 * - Aggiornamento delle modalità operative.
 * - Esecuzione del controllo di volo tramite PID.
 * - Calcolo e invio degli output agli attuatori.
 * - Stampa e invio dei log tramite server HTTP per il monitoraggio remoto.
 */
void loop()
{
    unsigned long t = millis();       // Timestamp attuale in millisecondi
    double dt = (t - tPrev) / 1000.0; // Calcolo del tempo trascorso in secondi
    tPrev = t;                        // Aggiorna il timestamp precedente

    // Lettura dei dati dai sensori di bordo
    systemController.read_imu();      // Aggiorna i dati dall'IMU
    systemController.read_receiver(); // Aggiorna i dati dal ricevitore RC

    // Verifica degli errori rilevati e aggiornamento dello stato del sistema
    systemController.check_errors();

    // Aggiornamento delle modalità operative in base agli input ricevuti
    systemController.update_modes();

    // Aggiornamento degli stati dei LED in base allo stato del sistema
    systemController.update_leds();

    // Esecuzione del controllo di volo (calcolo PID e logiche di stabilizzazione)
    systemController.control(dt);

    // Calcolo e invio degli output agli attuatori
    systemController.compute_output();

    // Stampa dei log formattati per il monitoraggio remoto
    logger->printFormattedLogs();
    logger->incrementCycleCount();
}
