#include "SystemController.h"
#include "Logger.h"
#include "prayers.h"
#include <Arduino.h>

/**
 * @brief Funzione generica per verificare se un valore è in un intervallo.
 */
template <typename T, typename U, typename V>
bool isInRange(T value, U min, V max)
{
    T lower = (min <= max) ? min : max;
    T upper = (min <= max) ? max : min;

    return value >= lower && value <= upper;
}

SystemController::SystemController() : state(CONTROLLER_STATE::DISARMED),
                                       assist_mode(ASSIST_MODE::MANUAL),
                                       controller_mode(CONTROLLER_MODE::STANDARD),
                                       calibration_target(CALIBRATION_TARGET::X)
{
    // Inizializza gli stati del sistema
    error.IMU_ERROR = false;
    error.RECEIVER_ERROR = false;
    Logger::getInstance().log(LogLevel::INFO, "System controller initialized.");
}

void SystemController::start()
{
    // Porta il sistema in stato armato
    state = CONTROLLER_STATE::ARMED;
    Logger::getInstance().log(LogLevel::INFO, "Controller state set -> ARMED.");
}

void SystemController::stop()
{
    // Porta il sistema in stato disarmato
    if (state == CONTROLLER_STATE::FAILSAFE)
        Logger::getInstance().log(LogLevel::WARNING, "Failsafe mode released.");
    state = CONTROLLER_STATE::DISARMED;
    Logger::getInstance().log(LogLevel::INFO, "Controller state set -> DISARMED.");
}

void SystemController::failSafe()
{
    // Porta il sistema in stato di failsafe
    state = CONTROLLER_STATE::FAILSAFE;
    Logger::getInstance().log(LogLevel::WARNING, "Controller state set -> FAILSAFE.");
}

bool SystemController::check_disarm_conditions(ReceiverData &receiver_data)
{
    // Verifica se le condizioni per disarmare il sistema sono soddisfatte
    return isInRange(receiver_data.z, YAW_MAX, YAW_MAX - YAW_MAX * ARM_TOLERANCE * 0.01) &&
           isInRange(receiver_data.x, ROLL_MIN, ROLL_MIN + ROLL_MAX * ARM_TOLERANCE * 0.01) &&
           (state == CONTROLLER_STATE::ARMED || state == CONTROLLER_STATE::FAILSAFE);
}

bool SystemController::check_arm_conditions(ReceiverData &receiver_data)
{
    // Verifica se le condizioni per armare il sistema sono soddisfatte
    return isInRange(receiver_data.z, YAW_MIN, YAW_MIN + YAW_MAX * ARM_TOLERANCE * 0.01) &&
           isInRange(receiver_data.x, ROLL_MAX, ROLL_MAX - ROLL_MAX * ARM_TOLERANCE * 0.01) &&
           (state == CONTROLLER_STATE::DISARMED);
}

Errors error_prev = {false, false};

void SystemController::check_errors()
{
    // Gestisce gli errori rilevati nel sistema
    if (error.IMU_ERROR == error_prev.IMU_ERROR && error.RECEIVER_ERROR == error_prev.RECEIVER_ERROR || state == CONTROLLER_STATE::DISARMED)
    {
        error_prev.IMU_ERROR = error.IMU_ERROR;
        error_prev.RECEIVER_ERROR = error.RECEIVER_ERROR;
        return;
    }
    else if (!error.IMU_ERROR && !error.RECEIVER_ERROR && state == CONTROLLER_STATE::FAILSAFE)
    {
        Logger::getInstance().log(LogLevel::WARNING, "Failsafe mode released.");
        state = CONTROLLER_STATE::ARMED;
    }
    else if (error.IMU_ERROR)
    {
        Logger::getInstance().log(LogLevel::ERROR, "IMU error detected.");
        assist_mode = ASSIST_MODE::MANUAL;
        Logger::getInstance().log(LogLevel::WARNING, "Assist mode set -> Manual");
        failSafe();
    }
    else if (error.RECEIVER_ERROR && state == CONTROLLER_STATE::ARMED)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Receiver error detected.");
        assist_mode = ASSIST_MODE::ATTITUDE_CONTROL;
        Logger::getInstance().log(LogLevel::WARNING, "Assist mode set -> Attitude control");
        failSafe();
    }
    error_prev.IMU_ERROR = error.IMU_ERROR;
    error_prev.RECEIVER_ERROR = error.RECEIVER_ERROR;
}

void SystemController::update_state(ReceiverData &receiver_data)
{
    // Aggiorna lo stato del sistema in base alle condizioni di armamento e disarmo
    if ((receiver_data.throttle >= THROTTLE_MIN && receiver_data.throttle <= THROTTLE_MIN + THROTTLE_MAX * ARM_TOLERANCE * 0.01) &&
        -receiver_data.y >= PITCH_MIN && -receiver_data.y <= PITCH_MIN + PITCH_MAX * ARM_TOLERANCE * 0.01)
    {
        if (check_disarm_conditions(receiver_data))
        {
            stop();
            return;
        }

        if (check_arm_conditions(receiver_data))
        {
            start();
            return;
        }
    }
}

void SystemController::update_modes(ReceiverData &receiver_data, bool imuSetupComplete)
{
    if (state == CONTROLLER_STATE::FAILSAFE)
        return;

    if (state == CONTROLLER_STATE::ARMED && !imuSetupComplete)
    {
        if (assist_mode != ASSIST_MODE::MANUAL)
        {
            assist_mode = ASSIST_MODE::MANUAL;
            Logger::getInstance().log(LogLevel::WARNING, "IMU not set up. Assist mode set -> Manual");
        }
        if (controller_mode != CONTROLLER_MODE::STANDARD)
        {
            controller_mode = CONTROLLER_MODE::STANDARD;
            Logger::getInstance().log(LogLevel::WARNING, "IMU not set up. Controller mode set -> Standard");
        }
        return;
    }

    struct AssistModeMapping
    {
        int swa;
        int swb;
        ASSIST_MODE mode;
        const char *message;
    };

    struct ControllerModeMapping
    {
        int swd;
        int swc;
        CONTROLLER_MODE mode;
        const char *message;
    };

    AssistModeMapping assist_modes[] = {
        {0, -1, ASSIST_MODE::MANUAL, "Assist mode set -> Manual"},
        {1, 0, ASSIST_MODE::GYRO_STABILIZED, "Assist mode set -> Gyro stabilized"},
        {1, 1, ASSIST_MODE::ATTITUDE_CONTROL, "Assist mode set -> Attitude control"},
    };

    ControllerModeMapping controller_modes[] = {
        {0, -1, CONTROLLER_MODE::STANDARD, "Controller mode set -> Standard"},
        {1, 0, CONTROLLER_MODE::KP_CALIBRATION, "Controller mode set -> KP calibration"},
        {1, 1, CONTROLLER_MODE::KI_CALIBRATION, "Controller mode set -> KI calibration"},
        {1, 2, CONTROLLER_MODE::KD_CALIBRATION, "Controller mode set -> KD calibration"},
    };

    for (const auto &mode : assist_modes)
    {
        if (receiver_data.swa == mode.swa &&
            (mode.swb == -1 || receiver_data.swb == mode.swb) &&
            assist_mode != mode.mode)
        {
            assist_mode = mode.mode;
            Logger::getInstance().log(LogLevel::INFO, mode.message);
            break;
        }
    }

    for (const auto &mode : controller_modes)
    {
        if (receiver_data.swd == mode.swd &&
            (mode.swc == -1 || receiver_data.swc == mode.swc) &&
            controller_mode != mode.mode)
        {
            controller_mode = mode.mode;
            Logger::getInstance().log(LogLevel::INFO, mode.message);
            break;
        }
    }
}

void SystemController::set_output(Output &output, ReceiverData &receiver_data, bool imuSetupComplete)
{
    static bool critical_error = false;
    // Aggiorna gli output in base allo stato del sistema
    if (state == CONTROLLER_STATE::DISARMED)
    {
        output = {0, 0, 0, 0};
        return;
    }
    else if (assist_mode == ASSIST_MODE::MANUAL)
    {
        output.x = receiver_data.x;
        output.y = receiver_data.y;
        output.z = receiver_data.z;
        output.throttle = receiver_data.throttle;
    }
    else if ((error.IMU_ERROR || !imuSetupComplete) && error.RECEIVER_ERROR)
    {
        output = {ITSRAININGMAN, HALLELUJAH, ITSRAININGMAN, HEYMAN};
        if(!critical_error){
            Logger::getInstance().log(LogLevel::ERROR, "Critical error detected. System halted. Prayers sent.");
            critical_error = true;
        }
        return;
    }
}
