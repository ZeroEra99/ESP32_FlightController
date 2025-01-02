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

/**
 * @class FlightController
 * @brief Classe che gestisce il controllo di volo, ricevendo input dai sensori
 *        e dagli utenti, e inviando output agli attuatori.
 */
class FlightController
{
private:
    /**
     * @brief Avvia il controller impostando lo stato a ARMED.
     */
    void start();

    /**
     * @brief Ferma il controller impostando lo stato a DISARMED.
     */
    void stop();

    /**
     * @brief Attiva la modalità failsafe in caso di errore critico.
     */
    void fail_safe();

    /**
     * @brief Gestisce errori critici impostando il controller per l'atterraggio automatico.
     */
    void critical_error();

    /**
     * @brief Legge i dati dall'IMU e aggiorna lo stato interno.
     */
    void read_imu();

    /**
     * @brief Legge i dati dal ricevitore e aggiorna i comandi del pilota.
     */
    void read_receiver();

    /**
     * @brief Aggiorna le modalità operative del controller.
     */
    void update_modes();

    /**
     * @brief Elabora i dati per il controllo PID.
     * 
     * @param dt Intervallo di tempo dall'ultimo aggiornamento.
     */
    void compute_data(double dt);

    /**
     * @brief Esegue il controllo utilizzando i valori PID calcolati.
     */
    void control();

    /**
     * @brief Invia i valori calcolati agli attuatori.
     */
    void output();

    // Componenti fisiche
    ESC esc;                  ///< Controlla il motore tramite segnali PWM.
    Motor servos[3];          ///< Array di servomotori per pitch, roll e yaw.
    Receiver receiver;        ///< Ricevitore per i comandi del pilota.
    BNO055 imu;               ///< Sensore IMU per orientamento e velocità.

    // Componenti logiche
    PIDcontrol pid_attitude[3]{
        PIDcontrol(KP_ATTITUDE_ROLL, KD_ATTITUDE_ROLL, KI_ATTITUDE_ROLL, MAX_INTEGRAL_ATTITUDE),
        PIDcontrol(KP_ATTITUDE_PITCH, KD_ATTITUDE_PITCH, KI_ATTITUDE_PITCH, MAX_INTEGRAL_ATTITUDE),
        PIDcontrol(KP_ATTITUDE_YAW, KD_ATTITUDE_YAW, KI_ATTITUDE_YAW, MAX_INTEGRAL_ATTITUDE)}; ///< PID per il controllo degli angoli.
    PIDcontrol pid_gyro[3]{
        PIDcontrol(KP_GYRO_ROLL, KD_GYRO_ROLL, KI_GYRO_ROLL, MAX_INTEGRAL_GYRO),
        PIDcontrol(KP_GYRO_PITCH, KD_GYRO_PITCH, KI_GYRO_PITCH, MAX_INTEGRAL_GYRO),
        PIDcontrol(KP_GYRO_YAW, KD_GYRO_YAW, KI_GYRO_YAW, MAX_INTEGRAL_GYRO)}; ///< PID per il controllo delle velocità angolari.
    ControllerData data;      ///< Struttura dati per memorizzare lo stato del controller.

    // Stati del controllore
    STATE state;              ///< Stato attuale del controller.
    ASSIST_MODE assist_mode;  ///< Modalità di assistenza corrente.
    CONTROLLER_MODE controller_mode; ///< Modalità di controllo corrente.
    Axis calibration_target;  ///< Asse target per la calibrazione PID.

public:
    /**
     * @brief Costruttore della classe FlightController.
     */
    FlightController();

    /**
     * @brief Esegue il ciclo principale di controllo del volo.
     */
    void control_loop();
};

#endif // FLIGHT_CONTROLLER_H