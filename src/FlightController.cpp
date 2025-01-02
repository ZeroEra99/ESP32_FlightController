#include "FlightController.h"
#include "Utils.h"

int receiver_pins[IA6B_CHANNELS] = {};

FlightController::FlightController() : aircraft(), receiver(Receiver(receiver_pins))
{
}

void FlightController::setup()
{
}

void FlightController::control_loop()
{
    Serial.print("Control loop starting.\n");
    while (true)
    {
        input();
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

void FlightController::input()
{
    // Leggi i dati dal ricevitore
    int pilot_data[IA6B_CHANNELS];                           // Array locale, gestito automaticamente
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
            isInRange(pilot_data[ROLL], PWM_MIN, PWM_MIN + PWM_MAX * ARM_TOLERANCE * 0.01))
            stop();
        else
        {
            if (isInRange(pilot_data[YAW], PWM_MIN, PWM_MIN + PWM_MAX * ARM_TOLERANCE * 0.01) &&
                isInRange(pilot_data[ROLL], PWM_MAX, PWM_MAX - PWM_MAX * ARM_TOLERANCE * 0.01))
                start();
        }
    }
    // Mappa i valori analogici in valori digitali
    pilot_data[ROLL] = pwm_to_digital(pilot_data[ROLL], PWM_MIN, PWM_MAX, -ROLL_MAX, ROLL_MAX); // Macro da definire
    pilot_data[PITCH] = pwm_to_digital(pilot_data[PITCH], PWM_MIN, PWM_MAX, -PITCH_MAX, PITCH_MAX);
    pilot_data[THROTTLE] = pwm_to_digital(pilot_data[THROTTLE], PWM_MIN, PWM_MAX, THROTTLE_MIN, THROTTLE_MAX);
    pilot_data[YAW] = pwm_to_digital(pilot_data[YAW], PWM_MIN, PWM_MAX, -YAW_MAX, YAW_MAX);
    pilot_data[AUX1] = map(pilot_data[AUX1], PWM_MIN, PWM_MAX, 0, 2);
    pilot_data[AUX2] = pwm_to_digital(pilot_data[AUX2], PWM_MIN, PWM_MAX, 0, PID_TUNING_MAX_VALUE);
    // Salva i dati dell'utente
    for (int i = 0; i < EULER_DIM; i++)
        data.user_data[i] = pilot_data[i];
    switch (pilot_data[AUX1])
    {
    case 0:
        // kp tune
        data.pid_tuning_offset[KP] = pilot_data[AUX2];
        break;
    case 1:
        // ki tune
        data.pid_tuning_offset[KI] = pilot_data[AUX2] / 5;
        break;
    case 2:
        // kd tune
        data.pid_tuning_offset[KD] = pilot_data[AUX2] / 8;
        break;
    }

    // Leggi i dati dall'IMU
    FlightData imu_data = aircraft.read_imu();
    // Salva i dati
    data.velocity[X] = imu_data.velocity[X];
    data.velocity[Y] = imu_data.velocity[Y];
    data.velocity[Z] = imu_data.velocity[Z];
    data.acceleration[X] = imu_data.acceleration[X];
    data.acceleration[Y] = imu_data.acceleration[Y];
    data.acceleration[Z] = imu_data.acceleration[Z];
    data.angular_velocities[X] = imu_data.angular_velocities[X];
    data.angular_velocities[Y] = imu_data.angular_velocities[Y];
    data.angular_velocities[Z] = imu_data.angular_velocities[Z];
    data.quaternion[W] = imu_data.quaternion[W];
    data.quaternion[X] = imu_data.quaternion[X];
    data.quaternion[Y] = imu_data.quaternion[Y];
    data.quaternion[Z] = imu_data.quaternion[Z];
}

void FlightController::compute_data(double dt)
{

    // Calcolo dell'errore di velocità angolare solo in modalità GYRO_STABILIZED
    if (mode == MODE::GYRO_STABILIZED)
    {
        for (int i = 0; i < EULER_DIM; i++)
        {
            data.error_angular_velocity[i] = data.user_data[i] - data.angular_velocities[i];
        }
    }

    // Modalità GUIDED: Modifica del setpoint quaternioni e calcolo dell'errore
    if (mode == MODE::GUIDED)
    {

        for (int i = 0; i < 3; i++) // Itera su ROLL, PITCH, YAW
        {
            if (data.user_data[i] != 0)
            {
                float q_rotation[4], q_temp[4];

                // Crea il quaternione per la rotazione
                quaternion_from_axis_angle(axis[i], data.user_data[i] * dt, q_rotation);

                // Moltiplica il setpoint quaternione con il quaternione di rotazione
                quaternion_multiply(q_rotation, data.setpoint_quaternion, q_temp);

                // Aggiorna il setpoint quaternione
                memcpy(data.setpoint_quaternion, q_temp, sizeof(q_temp));
            }
        }
        quaternion_normalize(data.setpoint_quaternion); // Normalizza il setpoint

        // Calcolo della differenza tra setpoint e IMU quaternione
        float q_conjugate[QUATERNION_DIM];
        float q_error[QUATERNION_DIM];
        quaternion_conjugate(data.quaternion, q_conjugate);                  // Coniugato del quaternione IMU
        quaternion_multiply(data.setpoint_quaternion, q_conjugate, q_error); // Differenza tra quaternioni
        for (int i = 0; i < QUATERNION_DIM; i++)
            data.error_angle[i] = q_error[i];
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

    if (mode == MODE::MANUAL)
    {
        // Nessun calcolo in modalità manuale
        return;
    }
    else if (mode == MODE::GYRO_STABILIZED)
    {
        // PID velocità angolare già implementato
        for (int i = 0; i < EULER_DIM; i++)
        {
            float pid_output = pid_angular_velocity[i].pid(data.error_angular_velocity[i], dt);
            data.pid_output[i] = pid_output;
        }
    }
    else if (mode == MODE::GUIDED)
    {
        // Calcolo PID per i quaternioni
        float pid_output_quaternion[QUATERNION_DIM - 1];
        for (int i = 1; i < QUATERNION_DIM; i++) // Ignora la componente scalare (q[0])
            pid_output_quaternion[i - 1] = pid_angle[i - 1].pid(data.error_angle[i], dt);

        // Passa l'output del PID quaternione come input al PID velocità angolare
        for (int i = 0; i < EULER_DIM; i++)
        {
            float angular_setpoint = pid_output_quaternion[i];
            float pid_output = pid_angular_velocity[i].pid(angular_setpoint - data.angular_velocities[i], dt);
            data.pid_output[i] = pid_output;
        }
    }
}

void FlightController::output()
{
    // Mappa i valori digitali in valori PWM
    int servo_values[EULER_DIM];
    servo_values[X] = digital_to_pwm(data.user_data[ROLL], -ROLL_MAX, ROLL_MAX, SERVO_MIN, SERVO_MAX);
    int throttle_value = digital_to_pwm(data.user_data[THROTTLE], THROTTLE_MIN, THROTTLE_MAX, PWM_MIN, PWM_MAX);
    // Scrivi i valori sugli attuatori
    for (int i = 0; i < EULER_DIM; i++)
        aircraft.servos[i].write(servo_values[i]);
    aircraft.throttle.write(throttle_value);
}


