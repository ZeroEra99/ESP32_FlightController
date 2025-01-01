#include "Aircraft.h"


Aircraft::Aircraft() : imu(), servos{Motor(SERVO_PIN_PITCH, SERVO_MIN, SERVO_MAX), Motor(SERVO_PIN_ROLL, SERVO_MIN, SERVO_MAX), Motor(SERVO_PIN_YAW, SERVO_MIN, SERVO_MAX)}, throttle(ESC(ESC_PIN, PWM_MAX, PWM_MAX)) {
}

FlightData Aircraft::read_imu() {
    return imu.read();
}