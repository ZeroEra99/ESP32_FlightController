/**
 * @file FlightController.h
 * @brief Dichiarazioni per la classe FlightController.
 *
 * La classe gestisce il controllo di volo, tra cui input del pilota, stabilizzazione
 * tramite PID, lettura dei sensori e output verso attuatori.
 */

#ifndef FLIGHT_CONTROLLER_H
#define FLIGHT_CONTROLLER_H

#include "Receiver.h"
#include "bno055.h"
#include "Motor.h"
#include "ESC.h"
#include "PIDcontrol.h"
#include "FlightControllerConfig.h"
#include "DebugLogger.h"

/**
 * @class FlightController
 * @brief Classe che gestisce il controllo di volo.
 *
 * Questa classe si occupa di:
 * - Ricevere input dagli utenti tramite il ricevitore RC.
 * - Leggere i dati dai sensori (IMU).
 * - Applicare algoritmi di controllo PID.
 * - Inviare comandi agli attuatori per stabilizzare e pilotare il sistema.
 */
class FlightController
{
private:
    /**
     * @brief Avvia il controller impostando lo stato a ARMED.
     *
     * Prepara il sistema per il volo attivando i componenti necessari.
     */
    void start();

    /**
     * @brief Ferma il controller impostando lo stato a DISARMED.
     *
     * Disattiva i componenti attivi per garantire la sicurezza.
     */
    void stop();

    /**
     * @brief Attiva la modalità failsafe in caso di errore critico.
     *
     * Imposta il controller in una modalità di emergenza per minimizzare i rischi.
     */
    void fail_safe();

    /**
     * @brief Gestisce errori critici impostando il controller per l'atterraggio automatico.
     *
     * Modifica lo stato e le modalità operative per eseguire un atterraggio sicuro.
     */
    void critical_error();

    /**
     * @brief Legge i dati dall'IMU e aggiorna lo stato interno.
     *
     * Acquisisce informazioni di orientamento, velocità angolare e accelerazione dal sensore IMU.
     */
    void read_imu();

    /**
     * @brief Legge i dati dal ricevitore e aggiorna i comandi del pilota.
     *
     * Converte i segnali PWM in input digitali per l'elaborazione successiva.
     */
    void read_receiver();

    /**
     * @brief Aggiorna le modalità operative del controller.
     *
     * Determina le modalità di assistenza e controllo attive in base agli input del pilota.
     */
    void update_modes();

    /**
     * @brief Elabora i dati per il controllo PID.
     *
     * Calcola i valori di errore e prepara i dati per il controllo PID.
     *
     * @param dt Intervallo di tempo dall'ultimo aggiornamento.
     */
    void compute_data(double dt);

    /**
     * @brief Esegue il controllo utilizzando i valori PID calcolati.
     *
     * Applica gli algoritmi PID per generare i comandi di controllo.
     */
    void control();

    /**
     * @brief Invia i valori calcolati agli attuatori.
     *
     * Converte i valori digitali in segnali PWM e li invia ai motori e servomotori.
     */
    void output();

    // Componenti fisiche
    ESC esc;           ///< Controlla il motore tramite segnali PWM.
    Motor servo_x;     ///< Servomotore per il rollio.
    Motor servo_y;     ///< Servomotore per il beccheggio.
    Motor servo_z;     ///< Servomotore per lo yaw.
    Receiver receiver; ///< Ricevitore per i comandi del pilota.
    BNO055 imu;        ///< Sensore IMU per orientamento e velocità.
    ReceiverPins receiver_pins = {
        IA6B_PIN_CHANNEL_1, IA6B_PIN_CHANNEL_2, IA6B_PIN_CHANNEL_3,
        IA6B_PIN_CHANNEL_4, IA6B_PIN_CHANNEL_5, IA6B_PIN_CHANNEL_6,
        IA6B_PIN_CHANNEL_7, IA6B_PIN_CHANNEL_8, IA6B_PIN_CHANNEL_9, IA6B_PIN_CHANNEL_10}; ///< Pin del ricevitore.

    // Componenti logiche
    PIDcontrol pid_attitude_x; ///< PID per il controllo dell'assetto sull'asse X.
    PIDcontrol pid_attitude_y; ///< PID per il controllo dell'assetto sull'asse Y.
    PIDcontrol pid_attitude_z; ///< PID per il controllo dell'assetto sull'asse Z.
    PIDcontrol pid_gyro_x;     ///< PID per il controllo del giroscopio sull'asse X.
    PIDcontrol pid_gyro_y;     ///< PID per il controllo del giroscopio sull'asse Y.
    PIDcontrol pid_gyro_z;     ///< PID per il controllo del giroscopio sull'asse Z.

    // Stati del controllore
    STATE state;                           ///< Stato attuale del controller.
    ASSIST_MODE assist_mode;               ///< Modalità di assistenza corrente.
    CONTROLLER_MODE controller_mode;       ///< Modalità di controllo corrente.
    CALIBRATION_TARGET calibration_target; ///< Asse target per la calibrazione PID.

    // Dati
    FlightData flight_data; ///< Dati di volo, inclusi accelerazione, velocità e attitudine.
    PilotData pilot_data;   ///< Dati del pilota, inclusi input da stick e interruttori.
    Euler error_gyro;       ///< Errore del giroscopio per ogni asse.
    Quaternion error_attitude; ///< Errore di attitudine rappresentato come quaternione.
    Quaternion setpoint_attitude; ///< Attitudine desiderata rappresentata come quaternione.
    PID pid_tuning_offset_gyro; ///< Offset per il tuning del PID del giroscopio.
    PID pid_tuning_offset_attitude; ///< Offset per il tuning del PID dell'assetto.
    AnalogOutput analog_output; ///< Uscite analogiche calcolate per gli attuatori.
    DigitalOutput digital_output; ///< Uscite digitali calcolate per gli attuatori.

public:
    /**
     * @brief Costruttore della classe FlightController.
     *
     * Inizializza tutte le componenti fisiche e logiche necessarie per il controllo di volo.
     */
    FlightController();
};

#endif // FLIGHT_CONTROLLER_H
