#include <Arduino.h>
#include "Aircraft.h"
#include "SystemController.h"
#include "FlightController.h"
#include "Logger.h"
#include "WiFiManager.h"

// Credenziali della rete Wi-Fi
bool usaReteCasa = true;                ///< Flag per l'utilizzo della rete Wi-Fi di casa
const char *ssidCasa = "TIM-19028281";  ///< SSID della rete Wi-Fi
const char *passwordCasa = "casa12345"; ///< Password della rete Wi-Fi
const char *ssidTelefono = "HONOR200";
const char *passwordTelefono = "moltobella";
const char *ssid = usaReteCasa ? ssidCasa : ssidTelefono;             ///< SSID della rete Wi-Fi
const char *password = usaReteCasa ? passwordCasa : passwordTelefono; ///< Password della rete Wi-Fi

const char *serverName = "Fede"; ///< Nome del server mDNS

Aircraft *aircraft = nullptr;
FlightController *flightController = nullptr;

SystemController systemController = SystemController();

static unsigned long tPrev = 0;               ///< Timestamp dell'ultimo ciclo in millisecondi.
static const unsigned long loopInterval = 10; ///< Intervallo del loop in millisecondi (100 Hz).

void setup()
{
    // Inizializzazione del monitor seriale
    Serial.begin(115200);

    // Inizializzazione del logger
    Logger::getInstance().startLogTask();

    // Inizializzazione delle connessioni
    WiFiManager::getInstance().begin(ssid, password);
    WiFiManager::getInstance().startConnectionTask();                // Avvia il task di connessione
    WiFiManager::getInstance().startServerDiscoveryTask(serverName); // Avvia il task di scoperta del server
    WiFiManager::getInstance().startServerCheckTask();               // Avvia il task di controllo dello stato del server

    // Inizializzazione del sistema di controllo
    aircraft = new Aircraft();
    flightController = new FlightController(aircraft->receiver_data, aircraft->imu_data, aircraft->output);

    Logger::getInstance().log(LogLevel::INFO, "Setup complete.");
}

void loop()
{
    unsigned long t = millis();    // Ottieni il timestamp attuale
    if (t - tPrev >= loopInterval) // Verifica se è passato l'intervallo necessario
    {
        double dt = (t - tPrev) / 1000.0; // Calcola l'intervallo di tempo in secondi
        tPrev = t;                        // Aggiorna il timestamp precedente

        // Aggiorna i dati del sistema
        aircraft->read_imu(systemController.error);
        aircraft->read_receiver(systemController.error);

        // Aggiorna lo stato del sistema
        systemController.update_state(aircraft->receiver_data);
        systemController.update_modes(aircraft->receiver_data, aircraft->imu.isSetupComplete);
        systemController.check_errors();

        // Calcola e applica il controllo
        flightController->compute_data(dt, aircraft->receiver_data, aircraft->imu_data, aircraft->output, systemController.assist_mode, systemController.state, systemController.error, systemController.controller_mode);
        flightController->control(dt, aircraft->imu_data, aircraft->receiver_data, aircraft->output, systemController.assist_mode, systemController.state, systemController.calibration_target);
        systemController.set_output(aircraft->output, aircraft->receiver_data, aircraft->imu.isSetupComplete);

        // Aggiorna i componenti hardware
        aircraft->update_leds(systemController.assist_mode, systemController.state);
        aircraft->write_actuators();

        // Aggiorna il logger
        aircraft->update_data_logger();
    }
}
