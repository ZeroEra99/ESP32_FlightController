/**
 * @file SystemController.h
 * @brief Definisce la classe SystemController per gestire l'intero sistema di volo.
 *
 * La classe SystemController coordina le operazioni di lettura dai sensori, controllo
 * tramite PID e output verso gli attuatori. Include la gestione di modalità operative,
 * stati di errore e la conversione dei dati digitali in segnali PWM.
 */

#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include "FlightController.h"
#include "Receiver.h"
#include "bno055.h"
#include "esc.h"
#include "motor.h"

/**
 * @class SystemController
 * @brief Classe principale per il controllo del sistema di volo.
 *
 * Questa classe coordina i vari componenti del sistema, inclusi sensori,
 * attuatori e il controller di volo. Gestisce:
 * - Lettura dei dati dal ricevitore RC e dalla IMU.
 * - Elaborazione dei dati di volo tramite il FlightController.
 * - Controllo degli attuatori sulla base dei segnali elaborati.
 */
class SystemController
{
private:
    /**
     * @brief Avvia il sistema di controllo.
     *
     * Imposta lo stato del sistema su ARMED, consentendo il controllo
     * completo degli attuatori e l'inizio del volo.
     */
    void start();

    /**
     * @brief Ferma il sistema di controllo.
     *
     * Imposta lo stato del sistema su DISARMED, bloccando l'output verso
     * gli attuatori per interrompere il volo.
     */
    void stop();

    /**
     * @brief Attiva la modalità failsafe in caso di errore critico.
     *
     * Questa funzione viene invocata per gestire errori come malfunzionamenti
     * dei sensori o perdita di controllo.
     *
     * @param error Tipo di errore rilevato.
     */
    void failsafe(ERROR_TYPE error);

    // Componenti fisiche
    ESC esc;           ///< Controlla il motore tramite segnali PWM.
    Motor servo_x;     ///< Servomotore per il controllo del rollio.
    Motor servo_y;     ///< Servomotore per il controllo del beccheggio.
    Motor servo_z;     ///< Servomotore per il controllo dell'imbardata.
    Receiver receiver; ///< Ricevitore RC per i comandi del pilota.
    BNO055 imu;        ///< Sensore IMU per l'orientamento e la velocità del sistema.

    // Componenti logiche
    FlightController flightController{
        pilot_data,
        flight_data,
        digital_output,
    }; ///< Controller di volo per l'elaborazione dei dati e il controllo PID.

    // Stati del controllore
    STATE state;                           ///< Stato attuale del controller.
    ASSIST_MODE assist_mode;               ///< Modalità di assistenza corrente.
    CONTROLLER_MODE controller_mode;       ///< Modalità di controllo corrente.
    ERROR_TYPE error_type;                 ///< Tipo di errore rilevato.
    CALIBRATION_TARGET calibration_target; ///< Asse target per la calibrazione PID.

    // Dati
    FlightData flight_data; ///< Dati di volo attuali (accelerazione, velocità, orientamento, ecc.).
    PilotData pilot_data;   ///< Input ricevuti dal pilota tramite il ricevitore RC.

    // Output
    DigitalOutput digital_output; ///< Valori digitali di output elaborati.
    AnalogOutput analog_output;   ///< Valori analogici di output mappati in segnali PWM.

public:
    /**
     * @brief Costruttore della classe SystemController.
     *
     * Inizializza i componenti fisici e logici, impostando lo stato iniziale
     * del sistema come DISARMED.
     */
    SystemController();

    /**
     * @brief Legge i dati dal ricevitore RC.
     *
     * Elabora i segnali ricevuti dal pilota e li converte in input digitali
     * per l'elaborazione successiva.
     */
    void read_receiver();

    /**
     * @brief Legge i dati dall'IMU.
     *
     * Raccoglie i dati di accelerazione, velocità angolare e orientamento dal sensore
     * IMU e li aggiorna nella struttura dei dati di volo.
     */
    void read_imu();

    /**
     * @brief Aggiorna le modalità operative del sistema.
     *
     * Determina e aggiorna le modalità di assistenza (es. manuale, stabilizzato)
     * e le modalità di controllo basandosi sull'input del pilota.
     */
    void update_modes();

    /**
     * @brief Aggiorna gli stati dei LED in base allo stato del sistema.
     *
     * Questo metodo si occupa di aggiornare i LED in base allo stato del sistema,
     * come ad esempio lo stato di ARMED/DISARMED o la presenza di errori.
     */
    void update_leds();

    /**
     * @brief Esegue il controllo di volo.
     *
     * Utilizza i dati raccolti e il FlightController per calcolare
     * gli output digitali da inviare agli attuatori.
     *
     * @param dt Intervallo di tempo dall'ultimo aggiornamento (in secondi).
     */
    void control(double dt);

    /**
     * @brief Converte i valori digitali in segnali PWM.
     *
     * Effettua la mappatura degli output digitali calcolati in segnali PWM
     * per il controllo degli attuatori, tenendo conto delle modalità operative.
     */
    void compute_output();
};

#endif // SYSTEM_CONTROLLER_H
