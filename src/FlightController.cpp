#include "FlightController.h"
#include "Quaternions.h"
#include "DebugLogger.h"

FlightController::FlightController(ReceiverData &receiver_data, ImuData &imu_data, Output &output)
    : pid_attitude_x(KP_ATTITUDE_X, KI_ATTITUDE_X, KD_ATTITUDE_X, MAX_INTEGRAL_ATTITUDE),
      pid_attitude_y(KP_ATTITUDE_Y, KI_ATTITUDE_Y, KD_ATTITUDE_Y, MAX_INTEGRAL_ATTITUDE),
      pid_attitude_z(KP_ATTITUDE_Z, KI_ATTITUDE_Z, KD_ATTITUDE_Z, MAX_INTEGRAL_ATTITUDE),
      pid_gyro_x(KP_GYRO_X, KI_GYRO_X, KD_GYRO_X, MAX_INTEGRAL_GYRO),
      pid_gyro_y(KP_GYRO_Y, KI_GYRO_Y, KD_GYRO_Y, MAX_INTEGRAL_GYRO),
      pid_gyro_z(KP_GYRO_Z, KI_GYRO_Z, KD_GYRO_Z, MAX_INTEGRAL_GYRO)
{
    // Inizializza gli errori e i parametri del controller di volo
    error_gyro = {0, 0, 0};
    error_attitude = {0, 0, 0, 0};
    desired_attitude = {1, 0, 0, 0};
    pid_tuning_offset_gyro = {0, 0, 0, 0};
    pid_tuning_offset_attitude = {0, 0, 0, 0};
    error = {0};
    DebugLogger::getInstance()->log("Flight controller initialized.", LogLevel::DEBUG);
}

void FlightController::compute_pid_offset(ASSIST_MODE assist_mode, CONTROLLER_MODE controller_mode, ReceiverData &receiver_data)
{
    // Aggiorna gli offset PID dinamici in base alla modalità operativa
    PID *target_pid = nullptr;

    if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
    {
        target_pid = &pid_tuning_offset_gyro;
    }
    else if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
    {
        target_pid = &pid_tuning_offset_attitude;
    }

    switch (controller_mode)
    {
    case CONTROLLER_MODE::KP_CALIBRATION:
        if (target_pid->kp == receiver_data.swb)
            return;
        target_pid->kp = receiver_data.swb;
        DebugLogger::getInstance()->log("Kp updated", LogLevel::DEBUG);
        break;
    case CONTROLLER_MODE::KI_CALIBRATION:
        if (target_pid->ki == receiver_data.swb)
            return;
        target_pid->ki = receiver_data.swb;
        DebugLogger::getInstance()->log("Ki updated", LogLevel::DEBUG);
        break;
    case CONTROLLER_MODE::KD_CALIBRATION:
        if (target_pid->kd == receiver_data.swb)
            return;
        target_pid->kd = receiver_data.swb;
        DebugLogger::getInstance()->log("Kd updated", LogLevel::DEBUG);
        break;
    default:
        break;
    }
}

void FlightController::compute_desired_attitude(float roll, float pitch, float yaw, Quaternion &result)
{
    // Calcola l'attitudine desiderata basandosi su input di rollio, beccheggio e imbardata
    Quaternion q_roll, q_pitch, q_yaw, quaternions[3];
    quaternion_from_axis_angle(axis[0], roll, q_roll);
    quaternion_from_axis_angle(axis[1], pitch, q_pitch);
    quaternion_from_axis_angle(axis[2], yaw, q_yaw);

    quaternions[0] = q_roll;
    quaternions[1] = q_pitch;
    quaternions[2] = q_yaw;
    quaternion_compose(quaternions, 3, result);
}

void FlightController::compute_data(double dt, ReceiverData &receiver_data, ImuData &imu_data,
                                    Output &output, ASSIST_MODE assist_mode,
                                    CONTROLLER_STATE state, Errors error, CONTROLLER_MODE controller_mode)
{
    if (assist_mode == ASSIST_MODE::MANUAL || error.IMU_ERROR)
    {
        return; // Nessuna elaborazione necessaria in modalità manuale o in caso di errore IMU
    }

    compute_pid_offset(assist_mode, controller_mode, receiver_data);

    if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
    {
        // Calcola gli errori angolari per la stabilizzazione giroscopica
        error_gyro.x = receiver_data.x - imu_data.gyro.x;
        error_gyro.y = receiver_data.y - imu_data.gyro.y;
        error_gyro.z = receiver_data.z - imu_data.gyro.z;
        return;
    }

    if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
    {
        // Calcola l'attitudine desiderata
        float roll = error.RECEIVER_ERROR ? AUTO_LAND_X : receiver_data.x;
        float pitch = error.RECEIVER_ERROR ? AUTO_LAND_Y : receiver_data.y;
        float yaw = error.RECEIVER_ERROR ? AUTO_LAND_Z : receiver_data.z;

        compute_desired_attitude(roll, pitch, yaw, desired_attitude);

        quaternion_normalize(desired_attitude);

        // Calcola l'errore tra attitudine desiderata e attuale
        quaternion_error(desired_attitude, imu_data.quat, error_attitude);
    }
}

void FlightController::compute_gyro_pid(const Euler &errors, const PID &pid_offsets, double dt,
                                        Output &output)
{
    // Calcola gli output PID per la velocità angolare
    output.x = pid_gyro_x.pid(errors.x, dt, pid_offsets.kp, pid_offsets.ki, pid_offsets.kd);
    output.y = pid_gyro_y.pid(errors.y, dt, pid_offsets.kp, pid_offsets.ki, pid_offsets.kd);
    output.z = pid_gyro_z.pid(errors.z, dt, pid_offsets.kp, pid_offsets.ki, pid_offsets.kd);
}

void FlightController::compute_attitude_pid(const Quaternion &errors, const PID &pid_offsets, double dt,
                                            Output &output)
{
    // Calcola gli output PID per l'attitudine
    output.x = pid_attitude_x.pid(errors.x, dt, pid_offsets.kp, pid_offsets.ki, pid_offsets.kd);
    output.y = pid_attitude_y.pid(errors.y, dt, pid_offsets.kp, pid_offsets.ki, pid_offsets.kd);
    output.z = pid_attitude_z.pid(errors.z, dt, pid_offsets.kp, pid_offsets.ki, pid_offsets.kd);
}

void FlightController::control(double dt, ImuData &imu_data, ReceiverData &receiver_data,
                               Output &output, ASSIST_MODE assist_mode,
                               CONTROLLER_STATE state, CALIBRATION_TARGET calibration_target)
{
    if (assist_mode == ASSIST_MODE::MANUAL || error.IMU_ERROR)
        return;

    if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
    {
        compute_gyro_pid(error_gyro, pid_tuning_offset_gyro, dt, output);
    }
    else if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
    {
        compute_attitude_pid(error_attitude, pid_tuning_offset_attitude, dt, output);
    }

    // Applica un fattore di riduzione per velocità elevate
    double forward_speed = imu_data.vel;
    double reduction_factor = 1.0;
    if (abs(forward_speed) > FORWARD_SPEED_THRESHOLD)
    {
        reduction_factor = SERVO_REDUCTION_FACTOR +
                           (1.0 - SERVO_REDUCTION_FACTOR) *
                               (FORWARD_SPEED_THRESHOLD / abs(forward_speed));
    }

    output.x *= reduction_factor;
    output.y *= reduction_factor;
    output.z *= reduction_factor;
}
