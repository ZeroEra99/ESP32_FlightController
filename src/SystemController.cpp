#include "SystemController.h"
#include "DebugLogger.h"
#include "prayers.h"

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
    error = {0};
    DebugLogger::getInstance()->log("System controller initialized.", LogLevel::DEBUG);
}

void SystemController::start()
{
    // Porta il sistema in stato armato
    state = CONTROLLER_STATE::ARMED;
    DebugLogger::getInstance()->log("Controller state set -> ARMED.", LogLevel::INFO);
}

void SystemController::stop()
{
    // Porta il sistema in stato disarmato
    if (state == CONTROLLER_STATE::FAILSAFE)
        DebugLogger::getInstance()->log("Failsafe mode forced release.", LogLevel::WARNING);

    DebugLogger::getInstance()->log("ESC stopped.", LogLevel::INFO);
    DebugLogger::getInstance()->log("Controller state set -> DISARMED.", LogLevel::INFO);
}

bool SystemController::check_disarm_conditions(ReceiverData &receiver_data)
{
    // Verifica se le condizioni per disarmare il sistema sono soddisfatte
    return isInRange(receiver_data.z, YAW_MAX, YAW_MAX - YAW_MAX * ARM_TOLERANCE * 0.01) &&
           isInRange(receiver_data.x, ROLL_MIN, ROLL_MIN + ROLL_MAX * ARM_TOLERANCE * 0.01) &&
           (state == CONTROLLER_STATE::ARMED);
}

bool SystemController::check_arm_conditions(ReceiverData &receiver_data)
{
    // Verifica se le condizioni per armare il sistema sono soddisfatte
    return isInRange(receiver_data.z, YAW_MIN, YAW_MIN + YAW_MAX * ARM_TOLERANCE * 0.01) &&
           isInRange(receiver_data.x, ROLL_MAX, ROLL_MAX - ROLL_MAX * ARM_TOLERANCE * 0.01) &&
           (state == CONTROLLER_STATE::DISARMED || state == CONTROLLER_STATE::FAILSAFE);
}

void SystemController::update_state(ReceiverData &receiver_data)
{
    // Aggiorna lo stato del sistema in base alle condizioni di armamento e disarmo
    if ((receiver_data.throttle > THROTTLE_MIN && receiver_data.throttle < THROTTLE_MIN + THROTTLE_MAX * ARM_TOLERANCE * 0.01) &&
        receiver_data.y > PITCH_MIN && receiver_data.y < PITCH_MIN + PITCH_MAX * ARM_TOLERANCE * 0.01)
    {
        if (check_disarm_conditions(receiver_data))
        {
            stop();
            return;
        }

        if (check_arm_conditions(receiver_data))
        {
            if (state == CONTROLLER_STATE::DISARMED)
                start();
            else if (state == CONTROLLER_STATE::FAILSAFE)
                stop();
            return;
        }
    }
}

void SystemController::update_modes(ReceiverData &receiver_data)
{
    // Aggiorna le modalità operative del sistema
    if (state == CONTROLLER_STATE::FAILSAFE)
        return;

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
            DebugLogger::getInstance()->log(mode.message, LogLevel::INFO);
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
            DebugLogger::getInstance()->log(mode.message, LogLevel::INFO);
            break;
        }
    }
}

void SystemController::check_errors()
{
    static Errors error_prev = {0}; //< Tipo di errore rilevato al ciclo precedente.
    // Gestisce gli errori rilevati nel sistema
    if (error.IMU_ERROR == error_prev.IMU_ERROR && error.RECEIVER_ERROR == error_prev.RECEIVER_ERROR)
        return;

    if (!error.IMU_ERROR && !error.RECEIVER_ERROR && state == CONTROLLER_STATE::FAILSAFE)
    {
        DebugLogger::getInstance()->log("Failsafe mode released.", LogLevel::WARNING);
        state = CONTROLLER_STATE::ARMED;
        return;
    }

    if (state != CONTROLLER_STATE::FAILSAFE)
    {
        DebugLogger::getInstance()->log("Failsafe mode set.", LogLevel::WARNING);
        state = CONTROLLER_STATE::FAILSAFE;
    }

    if (error.IMU_ERROR)
    {
        DebugLogger::getInstance()->log("IMU error detected.", LogLevel::ERROR);
        assist_mode = ASSIST_MODE::MANUAL;
    }
    else if (error.RECEIVER_ERROR)
    {
        DebugLogger::getInstance()->log("Pilot error detected.", LogLevel::ERROR);
        assist_mode = ASSIST_MODE::ATTITUDE_CONTROL;
    }
    error_prev = error;
}

void SystemController::set_output(Output output, ReceiverData &receiver_data)
{
    // Aggiorna gli output in base allo stato del sistema
    if (state == CONTROLLER_STATE::DISARMED)
    {
        output = {0, 0, 0, 0};
        return;
    }

    output.throttle = receiver_data.throttle;

    if (assist_mode == ASSIST_MODE::MANUAL)
    {
        output.x = receiver_data.x;
        output.y = receiver_data.y;
        output.z = receiver_data.z;
    }

    if (error.IMU_ERROR && error.RECEIVER_ERROR)
    {
        output = {ITSRAININGMAN, HALLELUJAH, ITSRAININGMAN, HEYMAN};
        return;
    }
}
