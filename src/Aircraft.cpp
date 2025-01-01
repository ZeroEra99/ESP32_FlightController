#include "Aircraft.h"

int receiver_pins[IA6B_CHANNELS] = {};

Aircraft::Aircraft() : receiver(receiver_pins), imu(), servos{Motor(SERVO_PIN_PITCH, SERVO_MIN, SERVO_MAX), Motor(SERVO_PIN_ROLL, SERVO_MIN, SERVO_MAX), Motor(SERVO_PIN_YAW, SERVO_MIN, SERVO_MAX)}, throttle(ESC(ESC_PIN, PWM_MAX, PWM_MAX)) {
}

void Aircraft::collect_data() {
    pilot_data = receiver.read();
    flight_data = imu.read();
}