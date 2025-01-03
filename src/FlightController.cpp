/**
 * @file FlightController.cpp
 * @brief Implementazione della classe FlightController.
 *
 * Questa classe gestisce il controllo di volo, compresi input del pilota,
 * stabilizzazione tramite PID, lettura dei sensori e output verso gli attuatori.
 */

#include "FlightController.h"
#include "Utils.h"

/**
 * @brief Inizializzazione dei pin del ricevitore.
 *
 * Array utilizzato per mappare i pin hardware del ricevitore RC.
 */
int receiver_pins[IA6B_CHANNELS] = {};

/**
 * @brief Costruttore della classe FlightController.
 *
 * Inizializza tutti i componenti fisici e logici del sistema, incluse le impostazioni
 * iniziali per PID, attuatori e dati di volo.
 */
FlightController::FlightController() : esc(ESC_PIN, PWM_MIN, PWM_MAX),
                                       servo_x(SERVO_PIN_X, SERVO_MIN, SERVO_MAX),
                                       servo_y(SERVO_PIN_Y, SERVO_MIN, SERVO_MAX),
                                       servo_z(SERVO_PIN_Z, SERVO_MIN, SERVO_MAX),
                                       receiver(receiver_pins),
                                       imu(),
                                       pid_attitude_x(KP_ATTITUDE_X, KI_ATTITUDE_X, KD_ATTITUDE_X, MAX_INTEGRAL_ATTITUDE),
                                       pid_attitude_y(KP_ATTITUDE_Y, KI_ATTITUDE_Y, KD_ATTITUDE_Y, MAX_INTEGRAL_ATTITUDE),
                                       pid_attitude_z(KP_ATTITUDE_Z, KI_ATTITUDE_Z, KD_ATTITUDE_Z, MAX_INTEGRAL_ATTITUDE),
                                       pid_gyro_x(KP_GYRO_X, KI_GYRO_X, KD_GYRO_X, MAX_INTEGRAL_GYRO),
                                       pid_gyro_y(KP_GYRO_Y, KI_GYRO_Y, KD_GYRO_Y, MAX_INTEGRAL_GYRO),
                                       pid_gyro_z(KP_GYRO_Z, KI_GYRO_Z, KD_GYRO_Z, MAX_INTEGRAL_GYRO)
{
    // Stato iniziale
    state = STATE::DISARMED;
    assist_mode = ASSIST_MODE::MANUAL;
    controller_mode = CONTROLLER_MODE::STANDARD;
    calibration_target = CALIBRATION_TARGET::X;

    // Inizializzazione dei dati del pilota
    pilot_data = {};

    // Inizializzazione dei dati di volo
    flight_data = {};

    // Inizializzazione dei parametri PID per il tuning
    pid_tuning_offset_attitude = {};
    pid_tuning_offset_gyro = {};

    // Inizializzazione degli errori
    error_gyro = {};
    error_attitude = {};
    setpoint_attitude = {1, 0, 0, 0}; // Quaternione unitario

    // Inizializzazione delle uscite
    analog_output = {};
    digital_output = {};
}

/**
 * @brief Avvia il controller impostando lo stato a ARMED.
 */
void FlightController::start()
{
    Serial.print("Starting flight controller.\n");
    state = STATE::ARMED;
    Serial.print("Flight controller started.\n");
}

/**
 * @brief Ferma il controller impostando lo stato a DISARMED.
 */
void FlightController::stop()
{
    Serial.print("Stopping flight controller.\n");
    state = STATE::DISARMED;
    Serial.print("Flight controller stopped.\n");
}

/**
 * @brief Attiva la modalità failsafe in caso di errore critico.
 *
 * Imposta il controller in modalità di emergenza per evitare ulteriori danni.
 */
void FlightController::fail_safe()
{
    Serial.print("Entering failsafe mode.\n");
    state = STATE::FAILSAFE;
    controller_mode = CONTROLLER_MODE::ERROR;
    assist_mode = ASSIST_MODE::MANUAL;
    Serial.print("Failsafe mode entered.\n");
}

/**
 * @brief Gestisce errori critici impostando il controller per l'atterraggio automatico.
 */
void FlightController::critical_error()
{
    Serial.print("Critical error detected.\n");
    state = STATE::AUTO_LAND;
    assist_mode = ASSIST_MODE::ATTITUDE_CONTROL;
}

/**
 * @brief Legge i dati dal ricevitore e li elabora per l'uso nel controllo di volo.
 *
 * Mappa i segnali PWM ricevuti in valori digitali per l'elaborazione.
 */
void FlightController::read_receiver()
{
    PilotData receiver_data = receiver.read();

    // Esegue controlli per armamento/disarmamento
    if (isInRange(receiver_data.throttle, PWM_MIN, PWM_MIN + PWM_MAX * ARM_TOLERANCE * 0.01) &&
        isInRange(receiver_data.y, PWM_MIN, PWM_MIN + PWM_MAX * ARM_TOLERANCE * 0.01))
    {
        if (isInRange(receiver_data.z, PWM_MAX, PWM_MAX - PWM_MAX * ARM_TOLERANCE * 0.01) &&
            isInRange(receiver_data.x, PWM_MIN, PWM_MIN + PWM_MAX * ARM_TOLERANCE * 0.01) &&
            state == STATE::ARMED)
        {
            stop();
        }
        else if (isInRange(receiver_data.z, PWM_MIN, PWM_MIN + PWM_MAX * ARM_TOLERANCE * 0.01) &&
                 isInRange(receiver_data.x, PWM_MAX, PWM_MAX - PWM_MAX * ARM_TOLERANCE * 0.01) &&
                 state == STATE::DISARMED)
        {
            start();
        }
    }

    // Mappa i valori ricevuti in valori digitali
    pilot_data.x = pwm_to_digital(receiver_data.x, PWM_MIN, PWM_MAX, -ROLL_MAX, ROLL_MAX);
    pilot_data.y = pwm_to_digital(receiver_data.y, PWM_MIN, PWM_MAX, -PITCH_MAX, PITCH_MAX);
    pilot_data.throttle = pwm_to_digital(receiver_data.throttle, PWM_MIN, PWM_MAX, THROTTLE_MIN, THROTTLE_MAX);
    pilot_data.z = pwm_to_digital(receiver_data.z, PWM_MIN, PWM_MAX, -YAW_MAX, YAW_MAX);
    pilot_data.swa = map(receiver_data.swa, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_ABC_MAX);
    pilot_data.swb = map(receiver_data.swb, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_ABC_MAX);
    pilot_data.swc = map(receiver_data.swc, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_ABC_MAX);
    pilot_data.swd = map(receiver_data.swd, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_D_MAX);
    pilot_data.vra = pwm_to_digital(receiver_data.vra, PWM_MIN, PWM_MAX, VRA_MIN, VRA_MAX);
    pilot_data.vrb = pwm_to_digital(receiver_data.vrb, PWM_MIN, PWM_MAX, VRB_MIN, VRB_MAX);
}

/**
 * @brief Legge i dati dall'IMU e aggiorna lo stato interno.
 *
 * Acquisisce i dati di accelerazione, velocità angolare e attitudine dall'IMU.
 */
void FlightController::read_imu()
{
    FlightData imu_data = imu.read();
    flight_data = imu_data; // Copia diretta dei dati dell'IMU
}

/**
 * @brief Aggiorna le modalità operative del controller basandosi sull'input dell'utente.
 *
 * Determina le modalità di assistenza e controllo attive in base ai dati del pilota.
 */
void FlightController::update_modes()
{
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
        if (pilot_data.swa == mode.swa &&
            (mode.swb == -1 || pilot_data.swb == mode.swb) &&
            assist_mode != mode.mode && state != STATE::FAILSAFE)
        {
            assist_mode = mode.mode;
            Serial.println(mode.message);
            break;
        }
    }

    for (const auto &mode : controller_modes)
    {
        if (pilot_data.swd == mode.swd &&
            (mode.swc == -1 || pilot_data.swc == mode.swc) &&
            controller_mode != mode.mode)
        {
            controller_mode = mode.mode;
            Serial.println(mode.message);
            break;
        }
    }
}
