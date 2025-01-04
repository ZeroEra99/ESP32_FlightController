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
    PID *target_pid = nullptr;

    // Determina quale PID deve essere aggiornato
    if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
    {
        target_pid = &pid_tuning_offset_gyro;
    }
    else if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
    {
        target_pid = &pid_tuning_offset_attitude;
    }

    // Aggiorna il parametro appropriato in base alla modalità di calibrazione
    switch (controller_mode)
    {
    case CONTROLLER_MODE::KP_CALIBRATION:
        target_pid->kp = pilot_data.swb;
        break;
    case CONTROLLER_MODE::KI_CALIBRATION:
        target_pid->ki = pilot_data.swb;
        break;
    case CONTROLLER_MODE::KD_CALIBRATION:
        target_pid->kd = pilot_data.swb;
        break;
    default:
        break;
    }
}

/**
 * @brief Calcola l'attitudine desiderata dai dati di rollio, beccheggio e imbardata.
 *
 * Converte gli input di rollio, beccheggio e imbardata in quaternioni e li compone
 * per ottenere l'attitudine desiderata.
 *
 * @param roll Valore del rollio.
 * @param pitch Valore del beccheggio.
 * @param yaw Valore dell'imbardata.
 * @param result Quaternione risultante.
 */
void FlightController::compute_desired_attitude(float roll, float pitch, float yaw, Quaternion &result)
{
    Quaternion q_roll, q_pitch, q_yaw, quaternions[3];
    quaternion_from_axis_angle(axis[0], roll, q_roll);
    quaternion_from_axis_angle(axis[1], pitch, q_pitch);
    quaternion_from_axis_angle(axis[2], yaw, q_yaw);

    quaternions[0] = q_roll;
    quaternions[1] = q_pitch;
    quaternions[2] = q_yaw;
    quaternion_compose(quaternions, 3, result);
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
void FlightController::compute_data(double dt, PilotData &pilot_data, FlightData &flight_data,
                                    DigitalOutput &digital_output, ASSIST_MODE assist_mode,
                                    STATE state, Errors error, CONTROLLER_MODE controller_mode)
{
    if (assist_mode == ASSIST_MODE::MANUAL || error.IMU_ERROR)
    {
        return;
    }

    compute_pid_offset(assist_mode, controller_mode, pilot_data);

    if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
    {
        error_gyro.x = pilot_data.x - flight_data.gyro.x;
        error_gyro.y = pilot_data.y - flight_data.gyro.y;
        error_gyro.z = pilot_data.z - flight_data.gyro.z;
        return; // Esce, poiché l'elaborazione dell'attitudine non è necessaria.
    }

    if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
    {
        // Calcola l'attitudine desiderata
        float roll = error.PILOT_ERROR ? AUTO_LAND_X : pilot_data.x;
        float pitch = error.PILOT_ERROR ? AUTO_LAND_Y : pilot_data.y;
        float yaw = error.PILOT_ERROR ? AUTO_LAND_Z : pilot_data.z;

        compute_desired_attitude(roll, pitch, yaw, desired_attitude);

        quaternion_normalize(desired_attitude);

        // Calcola l'errore tra attitudine desiderata e attuale
        quaternion_error(desired_attitude, flight_data.attitude, error_attitude);
    }
}

/**
 * @brief Calcola il controllo PID per la modalità GYRO_STABILIZED.
 *
 * @param errors Errori per i tre assi (X, Y, Z).
 * @param pid_offsets Offset PID per tuning dinamico.
 * @param dt Intervallo di tempo dall'ultimo aggiornamento (in secondi).
 * @param output Struttura in cui salvare gli output digitali.
 */
void FlightController::compute_gyro_pid(const Euler &errors, const PID &pid_offsets, double dt,
                                        DigitalOutput &output)
{
    output.x = pid_gyro_x.pid(errors.x, dt, pid_offsets.kp, pid_offsets.ki, pid_offsets.kd);
    output.y = pid_gyro_y.pid(errors.y, dt, pid_offsets.kp, pid_offsets.ki, pid_offsets.kd);
    output.z = pid_gyro_z.pid(errors.z, dt, pid_offsets.kp, pid_offsets.ki, pid_offsets.kd);
}

/**
 * @brief Calcola il controllo PID per la modalità ATTITUDE_CONTROL.
 *
 * @param errors Quaternione di errore tra l'attitudine desiderata e quella attuale.
 * @param pid_offsets Offset PID per tuning dinamico.
 * @param dt Intervallo di tempo dall'ultimo aggiornamento (in secondi).
 * @param output Struttura in cui salvare gli output digitali.
 */
void FlightController::compute_attitude_pid(const Quaternion &errors, const PID &pid_offsets, double dt,
                                            DigitalOutput &output)
{
    output.x = pid_attitude_x.pid(errors.x, dt, pid_offsets.kp, pid_offsets.ki, pid_offsets.kd);
    output.y = pid_attitude_y.pid(errors.y, dt, pid_offsets.kp, pid_offsets.ki, pid_offsets.kd);
    output.z = pid_attitude_z.pid(errors.z, dt, pid_offsets.kp, pid_offsets.ki, pid_offsets.kd);
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
void FlightController::control(double dt, FlightData &flight_data, PilotData &pilot_data,
                                DigitalOutput &digital_output, ASSIST_MODE assist_mode,
                                STATE state, CALIBRATION_TARGET calibration_target)
{
    if (assist_mode == ASSIST_MODE::MANUAL || error.IMU_ERROR)
        return;

    if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
    {
        compute_gyro_pid(error_gyro, pid_tuning_offset_gyro, dt, digital_output);
    }
    else if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
    {
        compute_attitude_pid(error_attitude, pid_tuning_offset_attitude, dt, digital_output);
    }
}
