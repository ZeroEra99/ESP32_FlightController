/**
 * @file ESP32_AircraftFlightController.ino
 * @brief Programma principale per il controllo del volo dell'aereo basato su ESP32.
 *
 * Questo file gestisce il ciclo principale del sistema, integrando letture dei sensori,
 * controllo degli attuatori, logging e gestione dell'Access Point Wi-Fi.
 */

#include <Arduino.h>
#include "SystemController.h"
#include "WiFiManager.h"
#include "Aircraft.h"
// #include "DebugLogger.h"

// Configurazione dell'Access Point Wi-Fi
const char *ssid = "ESP32_AP";

// Inizializzazione degli oggetti principali
Aircraft *aircraft = nullptr;
SystemController *systemController = nullptr;
FlightController *flightController = nullptr;
WiFiManager *wifiManager = nullptr;
// DebugLogger *logger = DebugLogger::getInstance();

static unsigned long tPrev = 0; ///< Timestamp dell'ultimo ciclo in millisecondi.

void setup()
{
    Serial.begin(115200); // Configura la porta seriale a 115200 baud

    // Inizializzazione degli oggetti principali
    aircraft = new Aircraft();
    systemController = new SystemController();
    flightController = new FlightController(aircraft->receiver_data, aircraft->imu_data, aircraft->output);
    wifiManager = new WiFiManager(ssid, "moltobella");
    wifiManager->manageAccessPoint(true);

    // Log iniziale per indicare l'avvio del sistema
    // logger->log("Flight controller started.", LogLevel::INFO);
}

void loop()
{
    unsigned long t = millis();       // Ottieni il timestamp attuale
    double dt = (t - tPrev) / 1000.0; // Calcola l'intervallo di tempo in secondi
    tPrev = t;                        // Aggiorna il timestamp precedente

    // Lettura dei sensori di bordo
    // aircraft->read_imu(systemController->error);
    aircraft->read_receiver(systemController->error);

    // Verifica degli errori e aggiornamento delle modalità operative
    // systemController->check_errors();
    // systemController->update_modes(aircraft->receiver_data);

    // Aggiorna lo stato dei LED in base alle modalità operative e agli errori
    // aircraft->update_leds(systemController->assist_mode, systemController->state, systemController->error);

    // Esegue il controllo di volo basato sui PID
    // flightController->control(dt, aircraft->imu_data, aircraft->receiver_data, aircraft->output,
    //                         systemController->assist_mode, systemController->state, systemController->calibration_target);

    // Aggiorna gli attuatori con i valori calcolati
    // systemController->set_output(aircraft->output, aircraft->receiver_data);
    // aircraft->write_actuators();

    Serial.print(aircraft->receiver_data.x);
    Serial.print("\t");
    Serial.print(aircraft->receiver_data.y);
    Serial.print("\t");
    Serial.print(aircraft->receiver_data.throttle);
    Serial.print("\t");
    Serial.print(aircraft->receiver_data.z);
    Serial.print("\t");
    Serial.print(aircraft->receiver_data.swa);
    Serial.print("\t");
    Serial.print(aircraft->receiver_data.swb);
    Serial.print("\t");
    Serial.print(aircraft->receiver_data.swc);
    Serial.print("\t");
    Serial.println(aircraft->receiver_data.swd);







    // Stampa i log per il monitoraggio remoto se il sistema è armato
    if (systemController->state != CONTROLLER_STATE::DISARMED)
        ;
    // logger->printFormattedLogs();

    // Gestisce l'Access Point in base allo stato del sistema
    // wifiManager->manageAccessPoint(systemController.state == CONTROLLER_STATE::DISARMED);

    // Incrementa il contatore dei cicli di log
    // logger->incrementCycleCount();
}
