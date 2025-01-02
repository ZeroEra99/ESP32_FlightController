/**
 * @file FlightController.cpp
 * @brief Implementazione della classe FlightController.
 */

#include "FlightController.h"
#include "Utils.h"

/**
 * @brief Inizializzazione dei pin del ricevitore.
 */
int receiver_pins[IA6B_CHANNELS] = {};

/**
 * @brief Costruttore della classe FlightController.
 */
FlightController::FlightController() : esc(ESC(ESC_PIN, PWM_MIN, PWM_MAX)), servos{Motor(SERVO_PIN_PITCH, SERVO_MIN, SERVO_MAX), Motor(SERVO_PIN_ROLL, SERVO_MIN, SERVO_MAX), Motor(SERVO_PIN_YAW, SERVO_MIN, SERVO_MAX)}, receiver(Receiver(receiver_pins)), imu(BNO055())
{
    state = STATE::DISARMED;
    assist_mode = ASSIST_MODE::MANUAL;
    controller_mode = CONTROLLER_MODE::STANDARD;
    calibration_target = Axis::X; //
    for (int i = 0; i < EULER_DIM; i++)
    {
        data.pid_tuning_offset_attitude[i] = 0;
        data.pid_tuning_offset_gyro[i] = 0;
        data.error_gyro[i] = 0;
        data.servo_output[i] = 0;
    }
    for (int i = 0; i < QUATERNION_DIM; i++)
    {
        data.setpoint_attitude[i] = 0;
        data.error_attitude[i] = 0;
    }
}

/**
 * @brief Esegue il ciclo principale di controllo.
 */
void FlightController::control_loop()
{
    Serial.print("Control loop starting.\n");
    while (true)
    {
        read_imu();
        read_receiver();
        update_modes();
        control();
    }
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
 */
void FlightController::read_receiver()
{
    {
        // Leggi i dati dal ricevitore
        float pilot_data[IA6B_CHANNELS];                         // Array locale, gestito automaticamente
        memcpy(pilot_data, receiver.read(), sizeof(pilot_data)); // Copia sicura dai dati del ricevitore
        for (int i = 0; i < IA6B_CHANNELS; i++)
        {
            if (std::isnan(pilot_data[i]))
            {
                i < FLIGHT_CHANNELS ? critical_error() : fail_safe();
            }
        }
        // Verifica i controlli per l'armamento
        if (isInRange(pilot_data[THROTTLE], PWM_MIN, PWM_MIN + PWM_MAX * ARM_TOLERANCE * 0.01) &&
            isInRange(pilot_data[PITCH], PWM_MIN, PWM_MIN + PWM_MAX * ARM_TOLERANCE * 0.01))
        {
            if (isInRange(pilot_data[YAW], PWM_MAX, PWM_MAX - PWM_MAX * ARM_TOLERANCE * 0.01) &&
                isInRange(pilot_data[ROLL], PWM_MIN, PWM_MIN + PWM_MAX * ARM_TOLERANCE * 0.01) &&
                state == STATE::ARMED)
            {
                for (int i = 0; i < EULER_DIM; i++)
                {
                    if (data.acceleration[i] > MOVE_ACCEL_THRESHOLD || data.velocity[i] > MOVE_SPEED_THRESHOLD)
                        return;
                }
                stop();
            }
            else
            {
                if (isInRange(pilot_data[YAW], PWM_MIN, PWM_MIN + PWM_MAX * ARM_TOLERANCE * 0.01) &&
                    isInRange(pilot_data[ROLL], PWM_MAX, PWM_MAX - PWM_MAX * ARM_TOLERANCE * 0.01) &&
                    state == STATE::DISARMED)
                    start();
            }
        }
        // Mappa i valori analogici stick in valori digitali
        pilot_data[ROLL] = pwm_to_digital(pilot_data[ROLL], PWM_MIN, PWM_MAX, -ROLL_MAX, ROLL_MAX); // Macro da definire
        pilot_data[PITCH] = pwm_to_digital(pilot_data[PITCH], PWM_MIN, PWM_MAX, -PITCH_MAX, PITCH_MAX);
        pilot_data[THROTTLE] = pwm_to_digital(pilot_data[THROTTLE], PWM_MIN, PWM_MAX, THROTTLE_MIN, THROTTLE_MAX);
        pilot_data[YAW] = pwm_to_digital(pilot_data[YAW], PWM_MIN, PWM_MAX, -YAW_MAX, YAW_MAX);
        // Mappa i valori analogici interruttori in valori digitali
        pilot_data[SWA] = map(pilot_data[SWA], PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_ABC_MAX);
        pilot_data[SWB] = map(pilot_data[SWB], PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_ABC_MAX);
        pilot_data[SWC] = map(pilot_data[SWC], PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_ABC_MAX);
        pilot_data[SWD] = map(pilot_data[SWD], PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_D_MAX);
        // Mappa i valori analogici potenziometri in valori digitali
        pilot_data[VRA] = pwm_to_digital(pilot_data[VRA], PWM_MIN, PWM_MAX, VRA_MIN, VRA_MAX);
        pilot_data[VRB] = pwm_to_digital(pilot_data[VRB], PWM_MIN, PWM_MAX, VRB_MIN, VRB_MAX);
    }
}

/**
 * @brief Legge i dati dall'IMU e aggiorna lo stato interno.
 */
void FlightController::read_imu()
{
    // Leggi i dati dall'IMU
    FlightData imu_data = imu.read();
    // Copia dei dati di velocità e accelerazione
    for (int i = 0; i < EULER_DIM; i++)
    {
        data.velocity[i] = imu_data.velocity[i];
        data.acceleration[i] = imu_data.acceleration[i];
        data.gyro[i] = imu_data.gyro[i];
    }
    // Copia dei dati di attitudine
    for (int i = 0; i < QUATERNION_DIM; i++)
        data.attitude[i] = imu_data.attitude[i];
}

/**
 * @brief Aggiorna le modalità operative del controller basandosi sull'input dell'utente.
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

    // Mappature per le modalità di assistenza
    AssistModeMapping assist_modes[] = {
        {0, -1, ASSIST_MODE::MANUAL, "Assist mode set -> Manual"},
        {1, 0, ASSIST_MODE::GYRO_STABILIZED, "Assist mode set -> Gyro stabilized"},
        {1, 1, ASSIST_MODE::ATTITUDE_CONTROL, "Assist mode set -> Attitude control"},
    };

    // Mappature per le modalità del controller
    ControllerModeMapping controller_modes[] = {
        {0, -1, CONTROLLER_MODE::STANDARD, "Controller mode set -> Standard"},
        {1, 0, CONTROLLER_MODE::KP_CALIBRATION, "Controller mode set -> KP calibration"},
        {1, 1, CONTROLLER_MODE::KI_CALIBRATION, "Controller mode set -> KI calibration"},
        {1, 2, CONTROLLER_MODE::KD_CALIBRATION, "Controller mode set -> KD calibration"},
    };

    // Controllo assist_mode
    for (const auto &mode : assist_modes)
    {
        if (data.user_input[SWA] == mode.swa &&
            (mode.swb == -1 || data.user_input[SWB] == mode.swb) &&
            assist_mode != mode.mode && state != STATE::FAILSAFE)
        {
            assist_mode = mode.mode;
            Serial.println(mode.message);
            break;
        }
    }

    // Controllo controller_mode
    for (const auto &mode : controller_modes)
    {
        if (data.user_input[SWD] == mode.swd &&
            (mode.swc == -1 || data.user_input[SWC] == mode.swc) &&
            controller_mode != mode.mode)
        {
            controller_mode = mode.mode;
            Serial.println(mode.message);
            break;
        }
    }
}

/**
 * @brief Calcola i dati di controllo basandosi sul tempo passato e sulle modalità correnti.
 * 
 * @param dt Intervallo di tempo dall'ultimo aggiornamento.
 */
void FlightController::compute_data(double dt)
{
    data.esc_output = data.user_input[THROTTLE];
    switch (controller_mode)
    {
    case CONTROLLER_MODE::STANDARD:
        break;
    case CONTROLLER_MODE::KP_CALIBRATION:
        if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
            data.pid_tuning_offset_gyro[KP] = data.user_input[SWB];
        if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
            data.pid_tuning_offset_attitude[KP] = data.user_input[SWB];
        break;
    case CONTROLLER_MODE::KI_CALIBRATION:
        if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
            data.pid_tuning_offset_gyro[KI] = data.user_input[SWB];
        if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
            data.pid_tuning_offset_attitude[KI] = data.user_input[SWB];
        break;
    case CONTROLLER_MODE::KD_CALIBRATION:
        if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
            data.pid_tuning_offset_gyro[KD] = data.user_input[SWB];
        if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
            data.pid_tuning_offset_attitude[KD] = data.user_input[SWB];
        break;
    default:
        break;
    }
    // Calcolo dell'errore di velocità angolare solo in modalità GYRO_STABILIZED
    if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
    {
        for (int i = 0; i < EULER_DIM; i++)
        {
            data.error_gyro[i] = data.user_input[i] - data.gyro[i];
        }
    }

    /**
     * Questa implementazione è temporanea, poiché attualmente l'imput utente modifica un quaternione di direzione desiderata.
     * Resterà così solo nelle fasi iniziali di calibrazione del PID.
     * In futuro, l'attitudine desiderata sarà rappresentata direttamente dall'input utente. E.g. comandi neutri, aereo vola dritto.
     */
    if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
    {

        if (state != STATE::AUTO_LAND)
        {
            for (int i = 0; i < 3; i++) // Itera su ROLL, PITCH, YAW
            {
                if (data.user_input[i] != 0)
                {
                    float q_rotation[4], q_temp[4];

                    // Crea il quaternione per la rotazione
                    quaternion_from_axis_angle(axis[i], data.user_input[i] * dt, q_rotation);

                    // Moltiplica il setpoint quaternione con il quaternione di rotazione
                    quaternion_multiply(q_rotation, data.setpoint_attitude, q_temp);

                    // Aggiorna il setpoint quaternione
                    memcpy(data.setpoint_attitude, q_temp, sizeof(q_temp));
                }
            }
        }
        else
        { // Imposta il quaternione desiderato per AUTO_LAND
            float auto_land_attitude[QUATERNION_DIM];
            float axis_roll[3] = {1, 0, 0};  // Asse X
            float axis_pitch[3] = {0, 1, 0}; // Asse Y

            float q_roll[QUATERNION_DIM], q_pitch[QUATERNION_DIM], q_temp[QUATERNION_DIM];

            // Creazione dei quaternioni per roll e pitch
            quaternion_from_axis_angle(axis_roll, AUTO_LAND_ROLL, q_roll);
            quaternion_from_axis_angle(axis_pitch, AUTO_LAND_PITCH, q_pitch);

            // Moltiplicazione dei quaternioni per ottenere il setpoint desiderato
            quaternion_multiply(q_roll, q_pitch, auto_land_attitude);

            // Aggiorna il setpoint di attitudine
            memcpy(data.setpoint_attitude, auto_land_attitude, sizeof(auto_land_attitude));
            data.esc_output = AUTO_LAND_THROTTLE;
        }

        // Calcolo della differenza tra setpoint e IMU quaternione
        quaternion_normalize(data.setpoint_attitude); // Normalizza il setpoint
        float q_conjugate[QUATERNION_DIM];
        float q_error[QUATERNION_DIM];
        quaternion_conjugate(data.attitude, q_conjugate);                  // Coniugato del quaternione IMU
        quaternion_multiply(data.setpoint_attitude, q_conjugate, q_error); // Differenza tra quaternioni
        for (int i = 0; i < QUATERNION_DIM; i++)
            data.error_attitude[i] = q_error[i];
        return;
    }
}

/**
 * @brief Esegue il controllo PID e aggiorna i valori per gli attuatori.
 */
void FlightController::control()
{
    static unsigned long tPrev = 0;
    unsigned long t = millis();
    double dt = (t - tPrev) / 1000.0;
    tPrev = t;

    compute_data(dt);

    if (assist_mode == ASSIST_MODE::MANUAL)
    {
        return;
    }
    else if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
    {
        // PID velocità angolare
        for (int i = 0; i < EULER_DIM; i++)
        {

            float pid_offset[EULER_DIM] = {0, 0, 0};
            if (calibration_target == i)
            {
                pid_offset[KP] = data.pid_tuning_offset_gyro[KP];
                pid_offset[KI] = data.pid_tuning_offset_gyro[KI];
                pid_offset[KD] = data.pid_tuning_offset_gyro[KD];
            }

            float pid_output = pid_gyro[i].pid(data.error_gyro[i], dt, pid_offset[KP], pid_offset[KI], pid_offset[KD]);
            data.servo_output[i] = pid_output;
        }
    }
    else if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
    {
        float pid_offset_quaternion[EULER_DIM] = {0, 0, 0};
        // Calcolo PID per i quaternioni
        float pid_output_quaternion[QUATERNION_DIM - 1];
               for (int i = 1; i < QUATERNION_DIM; i++)
        { // Ignora la componente scalare (q[0])
            if (calibration_target == i - 1)
            {
                pid_offset_quaternion[KP] = data.pid_tuning_offset_attitude[KP];
                pid_offset_quaternion[KI] = data.pid_tuning_offset_attitude[KI];
                pid_offset_quaternion[KD] = data.pid_tuning_offset_attitude[KD];
            }
            pid_output_quaternion[i - 1] = pid_attitude[i - 1].pid(data.error_attitude[i], dt, pid_offset_quaternion[KP], pid_offset_quaternion[KI], pid_offset_quaternion[KD]);
        }
        // Passa l'output del PID quaternione come input al PID velocità angolare
        for (int i = 0; i < EULER_DIM; i++)
        {
            float pid_offset_gyro[EULER_DIM] = {0, 0, 0};
            if (calibration_target == i)
            {
                pid_offset_gyro[KP] = data.pid_tuning_offset_gyro[KP];
                pid_offset_gyro[KI] = data.pid_tuning_offset_gyro[KI];
                pid_offset_gyro[KD] = data.pid_tuning_offset_gyro[KD];
            }
            float angular_setpoint = pid_output_quaternion[i];
            float pid_output = pid_gyro[i].pid(angular_setpoint - data.gyro[i], dt, pid_offset_gyro[KP], pid_offset_gyro[KI], pid_offset_gyro[KD]);
            data.servo_output[i] = pid_output;
        }
    }
}

/**
 * @brief Invia i valori calcolati agli attuatori.
 */
void FlightController::output()
{
    // Mappa i valori digitali in valori PWM
    int servo_values[EULER_DIM];
    int throttle_value;
    if (assist_mode != ASSIST_MODE::MANUAL && state != STATE::FAILSAFE && controller_mode != CONTROLLER_MODE::ERROR)
    {
        if (controller_mode != CONTROLLER_MODE::STANDARD)
        {
            switch (TUNING_TARGET_AXIS)
            {
            case X:
                servo_values[X] = digital_to_pwm(data.servo_output[X], -ROLL_MAX, ROLL_MAX, SERVO_MIN, SERVO_MAX);
                servo_values[Y] = digital_to_pwm(data.user_input[Y], -PITCH_MAX, PITCH_MAX, SERVO_MIN, SERVO_MAX);
                servo_values[Z] = digital_to_pwm(data.user_input[Z], -YAW_MAX, YAW_MAX, SERVO_MIN, SERVO_MAX);
                break;
            case Y:
                calibration_target = Axis::Y;
                servo_values[X] = digital_to_pwm(data.user_input[X], -ROLL_MAX, ROLL_MAX, SERVO_MIN, SERVO_MAX);
                servo_values[Y] = digital_to_pwm(data.servo_output[Y], -PITCH_MAX, PITCH_MAX, SERVO_MIN, SERVO_MAX);
                servo_values[Z] = digital_to_pwm(data.user_input[Z], -YAW_MAX, YAW_MAX, SERVO_MIN, SERVO_MAX);
                break;
            case Z:
                calibration_target = Axis::Z;
                servo_values[X] = digital_to_pwm(data.user_input[X], -ROLL_MAX, ROLL_MAX, SERVO_MIN, SERVO_MAX);
                servo_values[Y] = digital_to_pwm(data.user_input[Y], -PITCH_MAX, PITCH_MAX, SERVO_MIN, SERVO_MAX);
                servo_values[Z] = digital_to_pwm(data.servo_output[Z], -YAW_MAX, YAW_MAX, SERVO_MIN, SERVO_MAX);
                break;
            default:
                calibration_target = Axis::X;
                break;
            }
        }
        else
        {
            servo_values[X] = digital_to_pwm(data.servo_output[X], -ROLL_MAX, ROLL_MAX, SERVO_MIN, SERVO_MAX);
            servo_values[Y] = digital_to_pwm(data.servo_output[Y], -PITCH_MAX, PITCH_MAX, SERVO_MIN, SERVO_MAX);
            servo_values[Z] = digital_to_pwm(data.servo_output[Z], -YAW_MAX, YAW_MAX, SERVO_MIN, SERVO_MAX);
            throttle_value = digital_to_pwm(data.esc_output, THROTTLE_MIN, THROTTLE_MAX, PWM_MIN, PWM_MAX);
        }
    }
    else
    {
        servo_values[X] = digital_to_pwm(data.user_input[ROLL], -ROLL_MAX, ROLL_MAX, SERVO_MIN, SERVO_MAX);
        servo_values[Y] = digital_to_pwm(data.user_input[PITCH], -PITCH_MAX, PITCH_MAX, SERVO_MIN, SERVO_MAX);
        servo_values[Z] = digital_to_pwm(data.user_input[YAW], -YAW_MAX, YAW_MAX, SERVO_MIN, SERVO_MAX);
        throttle_value = digital_to_pwm(data.user_input[THROTTLE], THROTTLE_MIN, THROTTLE_MAX, PWM_MIN, PWM_MAX);
    }

    // Scrivi i valori sugli attuatori
    for (int i = 0; i < EULER_DIM; i++)
        servos[i].write(servo_values[i]);
    esc.write(throttle_value);
}
