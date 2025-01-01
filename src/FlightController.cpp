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

void FlightController::get_data()
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
    pilot_data[ROLL] = pwm_to_digital(pilot_data[ROLL], PWM_MIN, PWM_MAX, -ROLL_PILOT_MAX, ROLL_PILOT_MAX); // Macro da definire
    pilot_data[PITCH] = pwm_to_digital(pilot_data[PITCH], PWM_MIN, PWM_MAX, -PITCH_PILOT_MAX, PITCH_PILOT_MAX);
    pilot_data[THROTTLE] = pwm_to_digital(pilot_data[THROTTLE], PWM_MIN, PWM_MAX, THROTTLE_PILOT_MIN, THROTTLE_PILOT_MAX);
    pilot_data[YAW] = pwm_to_digital(pilot_data[YAW], PWM_MIN, PWM_MAX, -YAW_PILOT_MAX, YAW_PILOT_MAX);
    pilot_data[AUX1] = map(pilot_data[AUX1], PWM_MIN, PWM_MAX, 0, 2);
    pilot_data[AUX2] = pwm_to_digital(pilot_data[AUX2], PWM_MIN, PWM_MAX, 0, PID_TUNING_MAX_VALUE);

    // Applica gli offset di tuning
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
    // Calcola l'errore velocità angolare
    float error_angular_velocity[3] = {
        pilot_data[ROLL] - imu_data.angular_velocities[X],
        pilot_data[PITCH] - imu_data.angular_velocities[Y],
        pilot_data[YAW] - imu_data.angular_velocities[Z]};
    // Calcola l'errore input utente angoli sui tre assi - LATER
    // Calcola l'errore input utente quaternion - LATER
}

void FlightController::control()
{
    static unsigned long tPrev = 0;   // Tempo dell'ultimo ciclo
    unsigned long t = millis();       // Ottieni il tempo corrente
    double dt = (t - tPrev) / 1000.0; // Calcola il tempo trascorso in secondi
    tPrev = t;                        // Aggiorna il tempo dell'ultimo ciclo

    // Calcola i PID
    float pid_offset_gyro[EULER_DIM];
    for (int i = 0; i < EULER_DIM; i++)
    {
        pid_offset_gyro[i] = pid_angular_velocity[i].pid(data.error_angular_velocity[i], dt);
        // inversamente proporzionale alla velocità lineare
        // Riduzione inversamente proporzionale alla velocità
        float forward_speed = data.velocity[X]; // Supponendo che l'asse X sia la direzione in avanti
        float reduction_factor = 1.0;

        if (forward_speed > 0)
            reduction_factor = 1.0 - 0.5 * std::min(forward_speed / FORWARD_SPEED_THRESHOLD, 1.0);

        // Calcolo PID con riduzione
        pid_offset_gyro[i] = pid_angular_velocity[i].pid(data.error_angular_velocity[i], dt) * reduction_factor;
        data.pid_output[i] = pid_offset_gyro[i];
    }
    // Calcola i PID per gli angoli - LATER
    // Calcola i PID per i quaternion - LATER
}
