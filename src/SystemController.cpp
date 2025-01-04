#include "SystemController.h"
#include "Utils.h"
#include "DebugLogger.h"

/// @brief Porta seriale per la comunicazione con il ricevitore.
static HardwareSerial serialPort = IBUS_RX_PIN;

/**
 * @brief Costruttore della classe SystemController.
 *
 * Inizializza i componenti fisici e logici del sistema, imposta
 * i valori predefiniti per gli stati e i dati, e disarma il controller.
 */
SystemController::SystemController() : esc(ESC_PIN, PWM_MIN, PWM_MAX),
                                       servo_x(SERVO_PIN_X, SERVO_MIN, SERVO_MAX),
                                       servo_y(SERVO_PIN_Y, SERVO_MIN, SERVO_MAX),
                                       servo_z(SERVO_PIN_Z, SERVO_MIN, SERVO_MAX),
                                       receiver(serialPort),
                                       imu(),
                                       led_red(LED_PIN_RED),
                                       led_green(LED_PIN_GREEN),
                                       led_rgb(LED_PIN_RGB_RED, LED_PIN_RGB_GREEN, LED_PIN_RGB_BLUE),
                                       state(STATE::DISARMED),
                                       assist_mode(ASSIST_MODE::MANUAL),
                                       controller_mode(CONTROLLER_MODE::STANDARD),
                                       calibration_target(CALIBRATION_TARGET::X)
{
    state = STATE::DISARMED;
    assist_mode = ASSIST_MODE::MANUAL;
    controller_mode = CONTROLLER_MODE::STANDARD;
    calibration_target = CALIBRATION_TARGET::X;
    error = {0};
    imu_data = {0};
    receiver_data = {0};
    flight_data = {0};
    pilot_data = {0};
    digital_output = {0};
    analog_output = {0};
}

/**
 * @brief Avvia il controller impostando lo stato su ARMED.
 *
 * Consente il controllo completo degli attuatori e l'inizio del volo.
 */
void SystemController::start()
{
    DebugLogger::getInstance()->log("Starting flight controller.\n", LogLevel::INFO);
    if (state != STATE::FAILSAFE && state != STATE::ARMED)
        state = STATE::ARMED;
    DebugLogger::getInstance()->log("Flight controller started.\n", LogLevel::INFO);
}

/**
 * @brief Ferma il controller impostando lo stato su DISARMED.
 *
 * Disabilita l'output verso gli attuatori e interrompe il volo.
 */
void SystemController::stop()
{
    DebugLogger::getInstance()->log("Stopping flight controller.\n", LogLevel::INFO);
    if (state != STATE::DISARMED)
        state = STATE::DISARMED;
    DebugLogger::getInstance()->log("Flight controller stopped.\n", LogLevel::INFO);
}

bool SystemController::check_disarm_conditions()
{
    return isInRange(receiver_data.z, PWM_MAX, PWM_MAX - PWM_MAX * ARM_TOLERANCE * 0.01) &&
           isInRange(receiver_data.x, PWM_MIN, PWM_MIN + PWM_MAX * ARM_TOLERANCE * 0.01) &&
           (state == STATE::ARMED);
}

bool SystemController::check_arm_conditions()
{
    return isInRange(receiver_data.z, PWM_MIN, PWM_MIN + PWM_MAX * ARM_TOLERANCE * 0.01) &&
               isInRange(receiver_data.x, PWM_MAX, PWM_MAX - PWM_MAX * ARM_TOLERANCE * 0.01) &&
               state == STATE::DISARMED ||
           state == STATE::FAILSAFE;
}

void SystemController::map_receiver_to_pilot_data()
{
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
 * Acquisisce dati di accelerazione, velocità angolare, orientamento e velocità lineare,
 * copiandoli nella struttura dei dati di volo.
 */
void SystemController::read_imu()
{
    if (imu.read(imu_data))
    {
        error.IMU_ERROR = true;
        return;
    }
    error.IMU_ERROR = false;
    flight_data.acceleration = imu_data.acceleration;
    flight_data.gyro = imu_data.gyro;
    flight_data.velocity = imu_data.velocity;
    flight_data.attitude = imu_data.attitude;
}

/**
 * @brief Legge i dati dal ricevitore RC e li elabora.
 *
 * Traduce i segnali analogici in input digitali, verifica le condizioni per armare/disarmare
 * il sistema e aggiorna i dati del pilota.
 */
void SystemController::read_receiver()
{
    // Legge i dati dal ricevitore e verifica eventuali errori
    if (!receiver.read(receiver_data))
    {
        error.PILOT_ERROR = true;
        return;
    }
    error.PILOT_ERROR = false;

    // Verifica le condizioni per armare/disarmare il sistema
    if (isInRange(receiver_data.throttle, PWM_MIN, PWM_MIN + PWM_MAX * ARM_TOLERANCE * 0.01) &&
        isInRange(receiver_data.y, PWM_MIN, PWM_MIN + PWM_MAX * ARM_TOLERANCE * 0.01))
    {
        if (check_disarm_conditions())
        {
            stop();
        }
        else if (check_arm_conditions())
        {
            start();
        }
    }

    // Mappa i valori del ricevitore ai dati del pilota
    map_receiver_to_pilot_data();
}

#include "prayers.h"
/**
 * @brief Aggiorna le modalità operative del sistema.
 *
 * Determina le modalità di assistenza e controllo basandosi sull'input del pilota.
 */
void SystemController::update_modes()
{
    if (state == STATE::FAILSAFE)
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
        if (pilot_data.swa == mode.swa &&
            (mode.swb == -1 || pilot_data.swb == mode.swb) &&
            assist_mode != mode.mode)
        {
            assist_mode = mode.mode;
            DebugLogger::getInstance()->log(mode.message, LogLevel::INFO);
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
            DebugLogger::getInstance()->log(mode.message, LogLevel::INFO);
            break;
        }
    }
}

/**
 * @brief Verifica gli errori rilevati e aggiorna lo stato del sistema.
 *
 * Verifica gli errori rilevati e aggiorna lo stato del sistema in base alla presenza
 * di errori di comunicazione, di sensori o di input del pilota.
 */
void SystemController::check_errors()
{
    if (!error.IMU_ERROR && !error.PILOT_ERROR && state == STATE::FAILSAFE)
    {
        state = STATE::ARMED;
        return;
    }

    if (state != STATE::FAILSAFE)
    {
        state = STATE::FAILSAFE;
    }

    if (error.IMU_ERROR && error.PILOT_ERROR)
    {
        DebugLogger::getInstance()->log("IMU and pilot error detected.\n", LogLevel::ERROR);
        if (state != STATE::DISARMED)
            state = STATE::DISARMED; // Correzione: assegna lo stato DISARMED
    }

    if (error.IMU_ERROR)
    {
        DebugLogger::getInstance()->log("IMU error detected.\n", LogLevel::ERROR);
        if (assist_mode != ASSIST_MODE::MANUAL)
            assist_mode = ASSIST_MODE::MANUAL; // Aggiorna sempre la modalità a MANUAL
    }
    else if (error.PILOT_ERROR)
    {
        DebugLogger::getInstance()->log("Pilot error detected.\n", LogLevel::ERROR);
        if (assist_mode != ASSIST_MODE::ATTITUDE_CONTROL)
            assist_mode = ASSIST_MODE::ATTITUDE_CONTROL; // Aggiorna sempre la modalità a ATTITUDE_CONTROL
    }
}

/**
 * @brief Aggiorna gli stati dei LED in base allo stato del sistema.
 *
 * Aggiorna i LED in base allo stato del sistema, ad esempio per indicare
 * se il sistema è armato, disattivato o in modalità failsafe.
 */
void SystemController::update_leds()
{

    switch (state)
    {
    case STATE::ARMED:
        led_green.set_state(BLINK_ON, BLINK_OFF / 5);
        led_red.set_state(LightState::OFF);
        switch (assist_mode)
        {
        case ASSIST_MODE::MANUAL:
            led_rgb.set_state(LightState::ON, Color::WHITE);
            break;
        case ASSIST_MODE::GYRO_STABILIZED:
            led_rgb.set_state(LightState::ON, Color::LIGHT_BLUE);
            break;
        case ASSIST_MODE::ATTITUDE_CONTROL:
            led_rgb.set_state(LightState::ON, Color::PURPLE);
            break;
        }
        break;
    case STATE::DISARMED:
        led_green.set_state(BLINK_ON / 5, BLINK_OFF);
        led_red.set_state(LightState::OFF); // On per problemi di rete
        led_rgb.set_state(LightState::OFF); // Informazioni sul buffer
        break;
    case STATE::FAILSAFE:
        led_green.set_state(LightState::OFF);
        led_red.set_state(BLINK_ON / 5, BLINK_OFF / 3);
        if (error.PILOT_ERROR)
            led_rgb.set_state(BLINK_ON / 5, BLINK_OFF / 5, Color::WHITE);
        if (error.IMU_ERROR)
            led_rgb.set_state(BLINK_ON / 5, BLINK_OFF / 5, Color::LIGHT_BLUE);
        if (error.DATA_ERROR)
            led_rgb.set_state(BLINK_ON / 5, BLINK_OFF / 5, Color::PURPLE);
        break;
    }
}

/**
 * @brief Esegue il controllo di volo.
 *
 * Utilizza i dati attuali per calcolare gli output tramite il FlightController.
 *
 * @param dt Intervallo di tempo dall'ultimo aggiornamento (in secondi).
 */
void SystemController::control(double dt)
{
    flightController.control(dt, flight_data, pilot_data, digital_output, assist_mode, state, calibration_target);
}

/**
 * @brief Converte i valori calcolati in segnali PWM e li invia agli attuatori.
 *
 * Riduce l'output dei servo in base alla velocità in avanti, applicando un fattore di riduzione.
 */
void SystemController::compute_output()
{

    if (!error.IMU_ERROR)
    {
        // Mappa i valori digitali in valori PWM
        if (assist_mode != ASSIST_MODE::MANUAL)
        {

            if (controller_mode != CONTROLLER_MODE::STANDARD)
            {
                switch (calibration_target)
                {
                case CALIBRATION_TARGET::X:
                    analog_output.x = digital_to_pwm(digital_output.x, -ROLL_MAX, ROLL_MAX, SERVO_MIN, SERVO_MAX);
                    analog_output.y = digital_to_pwm(pilot_data.y, -PITCH_MAX, PITCH_MAX, SERVO_MIN, SERVO_MAX);
                    analog_output.z = digital_to_pwm(pilot_data.z, -YAW_MAX, YAW_MAX, SERVO_MIN, SERVO_MAX);
                    break;
                case CALIBRATION_TARGET::Y:
                    calibration_target = CALIBRATION_TARGET::Y;
                    analog_output.x = digital_to_pwm(digital_output.x, -ROLL_MAX, ROLL_MAX, SERVO_MIN, SERVO_MAX);
                    analog_output.y = digital_to_pwm(digital_output.y, -PITCH_MAX, PITCH_MAX, SERVO_MIN, SERVO_MAX);
                    analog_output.z = digital_to_pwm(pilot_data.z, -YAW_MAX, YAW_MAX, SERVO_MIN, SERVO_MAX);
                    break;
                case CALIBRATION_TARGET::Z:
                    calibration_target = CALIBRATION_TARGET::Z;
                    analog_output.z = digital_to_pwm(digital_output.x, -ROLL_MAX, ROLL_MAX, SERVO_MIN, SERVO_MAX);
                    analog_output.y = digital_to_pwm(digital_output.y, -PITCH_MAX, PITCH_MAX, SERVO_MIN, SERVO_MAX);
                    analog_output.z = digital_to_pwm(digital_output.z, -YAW_MAX, YAW_MAX, SERVO_MIN, SERVO_MAX);
                    break;
                default:
                    break;
                }
            }
            else
            {
                analog_output.x = digital_to_pwm(digital_output.x, -ROLL_MAX, ROLL_MAX, SERVO_MIN, SERVO_MAX);
                analog_output.y = digital_to_pwm(digital_output.y, -PITCH_MAX, PITCH_MAX, SERVO_MIN, SERVO_MAX);
                analog_output.z = digital_to_pwm(digital_output.z, -YAW_MAX, YAW_MAX, SERVO_MIN, SERVO_MAX);
                if (!error.PILOT_ERROR)
                    analog_output.throttle = digital_to_pwm(digital_output.throttle, THROTTLE_MIN, THROTTLE_MAX, PWM_MIN, PWM_MAX);
                else
                    analog_output.throttle = digital_to_pwm(AUTO_LAND_THROTTLE, THROTTLE_MIN, THROTTLE_MAX, PWM_MIN, PWM_MAX);
            }
        }
        else
        {
            analog_output.x = digital_to_pwm(pilot_data.x, -ROLL_MAX, ROLL_MAX, SERVO_MIN, SERVO_MAX);
            analog_output.y = digital_to_pwm(pilot_data.y, -PITCH_MAX, PITCH_MAX, SERVO_MIN, SERVO_MAX);
            analog_output.z = digital_to_pwm(pilot_data.z, -YAW_MAX, YAW_MAX, SERVO_MIN, SERVO_MAX);
            analog_output.throttle = digital_to_pwm(pilot_data.throttle, THROTTLE_MIN, THROTTLE_MAX, PWM_MIN, PWM_MAX);
        }
        // Calcola la velocità lungo l'asse di movimento in avanti
        double forward_speed = flight_data.forward_speed; // Supponendo che l'asse X rappresenti il movimento in avanti
        // Determina il fattore di riduzione
        double reduction_factor = 1.0;
        if (abs(forward_speed) > FORWARD_SPEED_THRESHOLD)
        {
            reduction_factor = SERVO_REDUCTION_FACTOR +
                               (1.0 - SERVO_REDUCTION_FACTOR) *
                                   (FORWARD_SPEED_THRESHOLD / abs(forward_speed));
        }
        // Applica il fattore di riduzione e il segno usando std::copysign
        analog_output.x = static_cast<int>(std::copysign(analog_output.x * reduction_factor, forward_speed));
        analog_output.y = static_cast<int>(std::copysign(analog_output.y * reduction_factor, forward_speed));
        analog_output.z = static_cast<int>(std::copysign(analog_output.z * reduction_factor, forward_speed));
    }

    if (!error.DATA_ERROR || state == STATE::DISARMED)
    {
        // Scrivi i valori sugli attuatori
        servo_x.write(analog_output.x);
        servo_y.write(analog_output.y);
        servo_z.write(analog_output.z);
        esc.write(analog_output.throttle);
    }
    else
    {
        // Vai a cercare il nastro per le riparazioni
        servo_x.write(ITSRAININGMAN);
        servo_y.write(HALLELUJAH);
        servo_z.write(ITSRAININGMAN);
        esc.write(HEYMAN);
    }
}
