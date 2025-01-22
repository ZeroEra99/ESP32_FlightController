#include <Arduino.h>
#include "Aircraft.h"
#include "SystemController.h"
#include "FlightController.h"
#include "Logger.h"
#include "WiFiManager.h"

// Credenziali della rete Wi-Fi
bool usaReteCasa = false;               ///< Flag per l'utilizzo della rete Wi-Fi di casa
const char *ssidCasa = "TIM-19028281";  ///< SSID della rete Wi-Fi
const char *passwordCasa = "casa12345"; ///< Password della rete Wi-Fi
const char *ssidTelefono = "HONOR200";
const char *passwordTelefono = "moltobella";
const char *ssid = usaReteCasa ? ssidCasa : ssidTelefono;             ///< SSID della rete Wi-Fi
const char *password = usaReteCasa ? passwordCasa : passwordTelefono; ///< Password della rete Wi-Fi

const char *serverName = "Fede"; ///< Nome del server mDNS

Aircraft *aircraft = nullptr;
FlightController *flightController = nullptr;

SystemController controller = SystemController();

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
        aircraft->read_imu(controller.error);
        aircraft->read_receiver(controller.error);

        // Aggiorna lo stato del sistema
        controller.update_state(aircraft->receiver_data);
        controller.update_modes(aircraft->receiver_data, aircraft->imu.isSetupComplete);
        controller.check_errors();

        // Calcola e applica il controllo
        flightController->compute_data(dt, aircraft->receiver_data, aircraft->imu_data, aircraft->output, controller.assist_mode, controller.state, controller.error, controller.controller_mode);
        flightController->control(dt, aircraft->imu_data, aircraft->receiver_data, aircraft->output, controller.assist_mode, controller.state, controller.calibration_target);
        controller.set_output(aircraft->output, aircraft->receiver_data, aircraft->imu.isSetupComplete);

        // Aggiorna i componenti hardware
        aircraft->write_actuators();
        aircraft->update_leds(controller.assist_mode, controller.state);

        // Aggiorna il logger
        aircraft->update_data_logger();
    }
}
