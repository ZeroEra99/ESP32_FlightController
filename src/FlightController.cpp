/**
 * @file FlightController.cpp
 * @brief Implementazione della classe FlightController.
 *
 * Questa classe si occupa di calcolare i dati di controllo basandosi sugli input
 * del pilota e sui dati di volo, utilizzando algoritmi PID per stabilizzare il velivolo.
 */

#include "FlightController.h"
#include "Utils.h"

/**
 * @brief Costruttore della classe FlightController.
 *
 * Inizializza i PID per il controllo degli assetti (attitude) e delle velocità angolari (gyro),
 * oltre a impostare i valori iniziali per gli errori e i quaternioni di attitudine desiderata.
 *
 * @param pilot_data Riferimento ai dati di input del pilota.
 * @param flight_data Riferimento ai dati di volo attuali.
 * @param digital_output Riferimento alla struttura per memorizzare gli output calcolati.
 */
FlightController::FlightController(PilotData &pilot_data, FlightData &flight_data, DigitalOutput &digital_output)
    : pid_attitude_x(KP_ATTITUDE_X, KI_ATTITUDE_X, KD_ATTITUDE_X, MAX_INTEGRAL_ATTITUDE),
      pid_attitude_y(KP_ATTITUDE_Y, KI_ATTITUDE_Y, KD_ATTITUDE_Y, MAX_INTEGRAL_ATTITUDE),
      pid_attitude_z(KP_ATTITUDE_Z, KI_ATTITUDE_Z, KD_ATTITUDE_Z, MAX_INTEGRAL_ATTITUDE),
      pid_gyro_x(KP_GYRO_X, KI_GYRO_X, KD_GYRO_X, MAX_INTEGRAL_GYRO),
      pid_gyro_y(KP_GYRO_Y, KI_GYRO_Y, KD_GYRO_Y, MAX_INTEGRAL_GYRO),
      pid_gyro_z(KP_GYRO_Z, KI_GYRO_Z, KD_GYRO_Z, MAX_INTEGRAL_GYRO)
{
    error_gyro = {0, 0, 0};
    error_attitude = {0, 0, 0, 0};
    desired_attitude = {1, 0, 0, 0};
    pid_tuning_offset_gyro = {0, 0, 0, 0};
    pid_tuning_offset_attitude = {0, 0, 0, 0};
    error = {0};
}

/**
 * @brief Calcola gli offset dinamici per il tuning dei PID.
 *
 * Questo metodo calcola gli offset dinamici per il tuning dei PID in base
 * alla modalità operativa del controller e all'asse target per la calibrazione.
 *
 * @param assist_mode Tipo di assistenza attiva.
 * @param controller_mode Modalità operativa del controller.
 */
void FlightController::compute_pid_offset(ASSIST_MODE assist_mode, CONTROLLER_MODE controller_mode, PilotData &pilot_data)
{
    switch (controller_mode)
    {
    case CONTROLLER_MODE::STANDARD:
        break;
    case CONTROLLER_MODE::KP_CALIBRATION:
        if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
            pid_tuning_offset_gyro.kp = pilot_data.swb;
        if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
            pid_tuning_offset_attitude.kp = pilot_data.swb;
        break;
    case CONTROLLER_MODE::KI_CALIBRATION:
        if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
            pid_tuning_offset_gyro.ki = pilot_data.swb;
        if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
            pid_tuning_offset_attitude.ki = pilot_data.swb;
        break;
    case CONTROLLER_MODE::KD_CALIBRATION:
        if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
            pid_tuning_offset_gyro.kd = pilot_data.swb;
        if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
            pid_tuning_offset_attitude.kd = pilot_data.swb;
        break;
    default:
        break;
    }
}

/**
 * @brief Calcola i dati di controllo basandosi sulle modalità operative e sull'input.
 *
 * Questo metodo aggiorna gli errori di velocità angolare e di attitudine,
 * e calcola l'attitudine desiderata basandosi sugli input del pilota.
 *
 * @param dt Intervallo di tempo dall'ultimo aggiornamento (in secondi).
 * @param pilot_data Input del pilota.
 * @param flight_data Dati di volo attuali.
 * @param digital_output Output digitali calcolati.
 * @param assist_mode Modalità di assistenza (manuale, stabilizzato, controllo attitudine).
 * @param state Stato attuale del sistema.
 * @param error Tipo di errore rilevato.
 * @param controller_mode Modalità operativa del controller.
 */
void FlightController::compute_data(double dt, PilotData &pilot_data, FlightData &flight_data, DigitalOutput &digital_output, ASSIST_MODE assist_mode, STATE state, Errors error, CONTROLLER_MODE controller_mode)
{
    if (assist_mode == ASSIST_MODE::MANUAL || error.IMU_ERROR)
        return;

    // Calcola gli offset dinamici per il tuning dei PID
    compute_pid_offset(assist_mode, controller_mode, pilot_data);

    // Calcolo dell'errore di velocità angolare in modalità GYRO_STABILIZED
    if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
    {
        error_gyro.x = pilot_data.x - flight_data.gyro.x;
        error_gyro.y = pilot_data.y - flight_data.gyro.y;
        error_gyro.z = pilot_data.z - flight_data.gyro.z;
    }

    // Calcolo dell'errore di attitudine in modalità ATTITUDE_CONTROL
    if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
    {
        if (!error.PILOT_ERROR)
        {
            // Calcolo dell'attitudine desiderata dai dati del pilota
            float axis_roll[3] = {1, 0, 0};
            float axis_pitch[3] = {0, 1, 0};
            float axis_yaw[3] = {0, 0, 1};
            Quaternion q_roll, q_pitch, q_yaw, q_temp;

            quaternion_from_axis_angle(axis_roll, pilot_data.x, q_roll);
            quaternion_from_axis_angle(axis_pitch, pilot_data.y, q_pitch);
            quaternion_from_axis_angle(axis_yaw, pilot_data.z, q_yaw);

            quaternion_multiply(q_roll, q_pitch, q_temp);
            quaternion_multiply(q_temp, q_yaw, desired_attitude);
        }
        else
        {
            // Imposta il setpoint per l'atterraggio automatico
            Quaternion q_roll, q_pitch, q_yaw, q_temp;
            float axis_roll[3] = {1, 0, 0};
            float axis_pitch[3] = {0, 1, 0};
            float axis_yaw[3] = {0, 0, 1};

            quaternion_from_axis_angle(axis_roll, AUTO_LAND_X, q_roll);
            quaternion_from_axis_angle(axis_pitch, AUTO_LAND_Y, q_pitch);
            quaternion_from_axis_angle(axis_yaw, AUTO_LAND_Z, q_yaw);
            quaternion_multiply(q_roll, q_pitch, q_temp);
            quaternion_multiply(q_temp, q_yaw, desired_attitude);
        }

        quaternion_normalize(desired_attitude);

        Quaternion q_conjugate, q_error;
        quaternion_conjugate(flight_data.attitude, q_conjugate);
        quaternion_multiply(desired_attitude, q_conjugate, q_error);

        error_attitude = q_error;
    }
}

/**
 * @brief Esegue il controllo PID e aggiorna gli output digitali per gli attuatori.
 *
 * Questo metodo utilizza i PID per stabilizzare il velivolo sulla base degli errori
 * di velocità angolare o attitudine, a seconda della modalità di assistenza.
 *
 * @param dt Intervallo di tempo dall'ultimo aggiornamento (in secondi).
 * @param flight_data Dati di volo attuali.
 * @param pilot_data Input del pilota.
 * @param digital_output Output digitali calcolati per gli attuatori.
 * @param assist_mode Modalità di assistenza.
 * @param state Stato attuale del sistema.
 * @param calibration_target Asse target per la calibrazione PID.
 */
void FlightController::control(double dt, FlightData &flight_data, PilotData &pilot_data, DigitalOutput &digital_output, ASSIST_MODE assist_mode, STATE state, CALIBRATION_TARGET calibration_target)
{
    if (assist_mode == ASSIST_MODE::MANUAL || error.IMU_ERROR)
        return;

    if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
    {
        // Controllo PID per velocità angolari
        float pid_output_x = pid_gyro_x.pid(error_gyro.x, dt, pid_tuning_offset_gyro.kp, pid_tuning_offset_gyro.ki, pid_tuning_offset_gyro.kd);
        float pid_output_y = pid_gyro_y.pid(error_gyro.y, dt, pid_tuning_offset_gyro.kp, pid_tuning_offset_gyro.ki, pid_tuning_offset_gyro.kd);
        float pid_output_z = pid_gyro_z.pid(error_gyro.z, dt, pid_tuning_offset_gyro.kp, pid_tuning_offset_gyro.ki, pid_tuning_offset_gyro.kd);

        digital_output.x = pid_output_x;
        digital_output.y = pid_output_y;
        digital_output.z = pid_output_z;
    }
    else if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
    {
        // Controllo PID per attitudine
        float pid_output_roll = pid_attitude_x.pid(error_attitude.x, dt, pid_tuning_offset_attitude.kp, pid_tuning_offset_attitude.ki, pid_tuning_offset_attitude.kd);
        float pid_output_pitch = pid_attitude_y.pid(error_attitude.y, dt, pid_tuning_offset_attitude.kp, pid_tuning_offset_attitude.ki, pid_tuning_offset_attitude.kd);
        float pid_output_yaw = pid_attitude_z.pid(error_attitude.z, dt, pid_tuning_offset_attitude.kp, pid_tuning_offset_attitude.ki, pid_tuning_offset_attitude.kd);

        digital_output.x = pid_output_roll;
        digital_output.y = pid_output_pitch;
        digital_output.z = pid_output_yaw;
    }
}
