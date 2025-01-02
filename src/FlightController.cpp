#include "FlightController.h"
#include "Utils.h"

int receiver_pins[IA6B_CHANNELS] = {};

FlightController::FlightController() : esc(ESC(ESC_PIN, PWM_MIN, PWM_MAX)), servos{Motor(SERVO_PIN_PITCH, SERVO_MIN, SERVO_MAX), Motor(SERVO_PIN_ROLL, SERVO_MIN, SERVO_MAX), Motor(SERVO_PIN_YAW, SERVO_MIN, SERVO_MAX)}, receiver(Receiver(receiver_pins)), imu(BNO055())
{
    state = STATE::DISARMED;
    assist_mode = ASSIST_MODE::MANUAL;
    controller_mode = CONTROLLER_MODE::STANDARD;
    data = ControllerData();
}

void FlightController::setup()
{
}

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

void FlightController::start()
{
    Serial.print("Starting flight controller.\n");
    state = STATE::ARMED;
    Serial.print("Flight controller started.\n");
}

void FlightController::stop()
{
    Serial.print("Stopping flight controller.\n");
    state = STATE::DISARMED;
    Serial.print("Flight controller stopped.\n");
}

void FlightController::fail_safe()
{
    Serial.print("Entering failsafe mode.\n");
    state = STATE::FAILSAFE;
    Serial.print("Failsafe mode entered.\n");
}

void FlightController::read_receiver()
{
    {
        // Leggi i dati dal ricevitore
        float pilot_data[IA6B_CHANNELS];                         // Array locale, gestito automaticamente
        memcpy(pilot_data, receiver.read(), sizeof(pilot_data)); // Copia sicura dai dati del ricevitore
        for (int i = 0; i < IA6B_CHANNELS; i++)
        {
            if (pilot_data[i] == NAN)
                fail_safe();
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

void FlightController::update_modes()
{
    // Modalità di assistenza (ASSIST_MODE)
    if (data.user_input[SWA] == 0 && assist_mode != ASSIST_MODE::MANUAL)
    {
        assist_mode = ASSIST_MODE::MANUAL;
    }
    else if (data.user_input[SWA] == 1 && data.user_input[SWB] == 0 && assist_mode != ASSIST_MODE::GYRO_STABILIZED)
    {
        assist_mode = ASSIST_MODE::GYRO_STABILIZED;
    }
    else if (data.user_input[SWA] == 1 && data.user_input[SWB] == 1 && assist_mode != ASSIST_MODE::ATTITUDE_CONTROL)
    {
        assist_mode = ASSIST_MODE::ATTITUDE_CONTROL;
    }

    // Modalità del controller (CONTROLLER_MODE)
    if (data.user_input[SWD] == 0 && controller_mode != CONTROLLER_MODE::STANDARD)
    {
        controller_mode = CONTROLLER_MODE::STANDARD;
    }
    else if (data.user_input[SWD] == 1)
    {
        if (data.user_input[SWC] == 0 && controller_mode != CONTROLLER_MODE::KP_CALIBRATION)
        {
            controller_mode = CONTROLLER_MODE::KP_CALIBRATION;
        }
        else if (data.user_input[SWC] == 1 && controller_mode != CONTROLLER_MODE::KI_CALIBRATION)
        {
            controller_mode = CONTROLLER_MODE::KI_CALIBRATION;
        }
        else if (data.user_input[SWC] == 2 && controller_mode != CONTROLLER_MODE::KD_CALIBRATION)
        {
            controller_mode = CONTROLLER_MODE::KD_CALIBRATION;
        }
    }
}

void FlightController::compute_data(double dt)
{

    // Calcolo dell'errore di velocità angolare solo in modalità GYRO_STABILIZED
    if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
    {
        for (int i = 0; i < EULER_DIM; i++)
        {
            data.error_gyro[i] = data.user_input[i] - data.gyro[i];
        }
    }

    // Modalità GUIDED: Modifica del setpoint quaternioni e calcolo dell'errore
    if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
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
        quaternion_normalize(data.setpoint_attitude); // Normalizza il setpoint

        // Calcolo della differenza tra setpoint e IMU quaternione
        float q_conjugate[QUATERNION_DIM];
        float q_error[QUATERNION_DIM];
        quaternion_conjugate(data.attitude, q_conjugate);                  // Coniugato del quaternione IMU
        quaternion_multiply(data.setpoint_attitude, q_conjugate, q_error); // Differenza tra quaternioni
        for (int i = 0; i < QUATERNION_DIM; i++)
            data.error_attitude[i] = q_error[i];
        return;
    }
}

void FlightController::control()
{
    static unsigned long tPrev = 0;
    unsigned long t = millis();
    double dt = (t - tPrev) / 1000.0;
    tPrev = t;

    compute_data(dt);

    if (assist_mode == ASSIST_MODE::MANUAL)
    {
        // Nessun calcolo in modalità manuale
        return;
    }
    else if (assist_mode == ASSIST_MODE::GYRO_STABILIZED)
    {
        // PID velocità angolare già implementato
        for (int i = 0; i < EULER_DIM; i++)
        {
            float pid_output = pid_gyro[i].pid(data.error_gyro[i], dt);
            data.pid_output[i] = pid_output;
        }
    }
    else if (assist_mode == ASSIST_MODE::ATTITUDE_CONTROL)
    {
        // Calcolo PID per i quaternioni
        float pid_output_quaternion[QUATERNION_DIM - 1];
        for (int i = 1; i < QUATERNION_DIM; i++) // Ignora la componente scalare (q[0])
            pid_output_quaternion[i - 1] = pid_attitude[i - 1].pid(data.error_attitude[i], dt);

        // Passa l'output del PID quaternione come input al PID velocità angolare
        for (int i = 0; i < EULER_DIM; i++)
        {
            float angular_setpoint = pid_output_quaternion[i];
            float pid_output = pid_gyro[i].pid(angular_setpoint - data.gyro[i], dt);
            data.pid_output[i] = pid_output;
        }
    }
}

void FlightController::output()
{
    // Mappa i valori digitali in valori PWM
    int servo_values[EULER_DIM];
    servo_values[X] = digital_to_pwm(data.user_input[ROLL], -ROLL_MAX, ROLL_MAX, SERVO_MIN, SERVO_MAX);
    int throttle_value = digital_to_pwm(data.user_input[THROTTLE], THROTTLE_MIN, THROTTLE_MAX, PWM_MIN, PWM_MAX);
    // Scrivi i valori sugli attuatori
    for (int i = 0; i < EULER_DIM; i++)
        servos[i].write(servo_values[i]);
    esc.write(throttle_value);
}
