/**
 * @file FlightController.h
 * @brief Dichiarazione della classe FlightController per la gestione del controllo di volo.
 */

#ifndef FLIGHT_CONTROLLER_H
#define FLIGHT_CONTROLLER_H

#include "PIDcontroller.h"
#include "DataStructures.h"
#include "FlightControllerConfig.h"

/**
 * @brief Classe per la gestione del controller di volo.
 * 
 * Si occupa della logica di controllo di volo, includendo il calcolo dei PID per la stabilizzazione
 * e l'attitudine del velivolo.
 */
class FlightController
{
private:
    /**
     * @brief Calcola gli offset PID dinamici per il tuning.
     * 
     * @param assist_mode Modalità di assistenza corrente.
     * @param controller_mode Modalità di controllo corrente.
     * @param receiver_data Dati ricevuti dal pilota.
     */
    void compute_pid_offset(ASSIST_MODE assist_mode, CONTROLLER_MODE controller_mode, ReceiverData &receiver_data);

    /**
     * @brief Calcola l'attitudine desiderata in base agli input del pilota.
     * 
     * @param roll Rollio desiderato.
     * @param pitch Beccheggio desiderato.
     * @param yaw Imbardata desiderata.
     * @param result Quaternione risultante dell'attitudine desiderata.
     */
    void compute_desired_attitude(float roll, float pitch, float yaw, Quaternion &result);

    /**
     * @brief Calcola il controllo PID per le velocità angolari.
     * 
     * @param errors Errori angolari.
     * @param pid_offsets Offset PID dinamici.
     * @param dt Intervallo di tempo dall'ultimo ciclo.
     * @param output Struttura di output per gli attuatori.
     */
    void compute_gyro_pid(const Euler &errors, const PID &pid_offsets, double dt, Output &output);

    /**
     * @brief Calcola il controllo PID per l'attitudine.
     * 
     * @param errors Errori di attitudine (quaternione).
     * @param pid_offsets Offset PID dinamici.
     * @param dt Intervallo di tempo dall'ultimo ciclo.
     * @param output Struttura di output per gli attuatori.
     */
    void compute_attitude_pid(const Quaternion &errors, const PID &pid_offsets, double dt, Output &output);

    // Componenti logiche
    PIDcontroller pid_attitude_x; ///< PID per il controllo dell'assetto sull'asse X (rollio).
    PIDcontroller pid_attitude_y; ///< PID per il controllo dell'assetto sull'asse Y (beccheggio).
    PIDcontroller pid_attitude_z; ///< PID per il controllo dell'assetto sull'asse Z (imbardata).
    PIDcontroller pid_gyro_x;     ///< PID per il controllo della velocità angolare sull'asse X (rollio).
    PIDcontroller pid_gyro_y;     ///< PID per il controllo della velocità angolare sull'asse Y (beccheggio).
    PIDcontroller pid_gyro_z;     ///< PID per il controllo della velocità angolare sull'asse Z (imbardata).

    // Dati del controller di volo
    Euler error_gyro;               ///< Errori delle velocità angolari per ciascun asse (X, Y, Z).
    Quaternion error_attitude;      ///< Errori di attitudine calcolati come differenza tra setpoint e attitudine attuale.
    Quaternion desired_attitude;    ///< Attitudine desiderata calcolata dagli input del pilota.
    PID pid_tuning_offset_gyro;     ///< Offset dinamici per il tuning del PID delle velocità angolari.
    PID pid_tuning_offset_attitude; ///< Offset dinamici per il tuning del PID degli assetti.

    Errors error; ///< Struttura per gli errori rilevati.

public:
    /**
     * @brief Costruttore del FlightController.
     * 
     * @param receiver_data Riferimento ai dati ricevuti dal pilota.
     * @param imu_data Riferimento ai dati letti dall'IMU.
     * @param output Riferimento alla struttura di output per gli attuatori.
     */
    FlightController(ReceiverData &receiver_data, ImuData &imu_data, Output &output);

    /**
     * @brief Salva i dati di output del controller.
     * 
     * @param output Dati di output del controller.
     */
    void logData(const Output &output);

    /**
     * @brief Elabora i dati per la stabilizzazione del volo.
     * 
     * @param dt Intervallo di tempo dall'ultimo ciclo.
     * @param receiver_data Dati ricevuti dal pilota.
     * @param imu_data Dati letti dall'IMU.
     * @param output Struttura di output per gli attuatori.
     * @param assist_mode Modalità di assistenza corrente.
     * @param state Stato del controller.
     * @param error Errori attuali.
     * @param controller_mode Modalità di controllo corrente.
     */
    void compute_data(double dt, ReceiverData &receiver_data, ImuData &imu_data, Output &output, ASSIST_MODE assist_mode, CONTROLLER_STATE state, Errors error, CONTROLLER_MODE controller_mode);

    /**
     * @brief Esegue il controllo del velivolo.
     * 
     * @param dt Intervallo di tempo dall'ultimo ciclo.
     * @param imu_data Dati letti dall'IMU.
     * @param receiver_data Dati ricevuti dal pilota.
     * @param output Struttura di output per gli attuatori.
     * @param assist_mode Modalità di assistenza corrente.
     * @param state Stato del controller.
     * @param calibration_target Asse target per la calibrazione PID.
     */
    void control(double dt, ImuData &imu_data, ReceiverData &receiver_data, Output &output, ASSIST_MODE assist_mode, CONTROLLER_STATE state, CALIBRATION_TARGET calibration_target);
};

#endif // FLIGHT_CONTROLLER_H
