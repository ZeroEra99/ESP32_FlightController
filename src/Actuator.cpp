#include "Actuator.h"
#include "HardwareParameters.h"
#include "Logger.h"
#include <Arduino.h>

/**
 * Funzione per limitare un valore all'interno di un intervallo.
 */
template <typename T>
constexpr const T &clamp(const T &value, const T &min, const T &max)
{
    return (value < min) ? min : (value > max) ? max
                                               : value;
}

/**
 * Converte un valore digitale in PWM, rispettando i limiti configurati.
 */
int digital_to_pwm(double value, double min_digital, double max_digital, int min_analog, int max_analog)
{
    return clamp(static_cast<int>(((value - min_digital) * (max_analog - min_analog) / (max_digital - min_digital)) + min_analog), min_analog, max_analog);
}

ESC::ESC(int pin)
    : pin(pin)
{
    esc.attach(pin, PWM_MIN, PWM_MAX);
    esc.writeMicroseconds(PWM_MIN);

    Logger::getInstance().log(LogLevel::INFO, "ESC setup complete.");
}

void ESC::writeESC(double value)
{
    int pwm_value = digital_to_pwm(value, THROTTLE_MIN, THROTTLE_MAX, PWM_MIN, PWM_MAX);
    esc.writeMicroseconds(pwm_value);
}

ServoMotor::ServoMotor(int pin) : pin(pin)
{
    servo.attach(pin);

    int mid_pwm = digital_to_pwm(0, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, 0, 180);
    servo.write(mid_pwm);

    Logger::getInstance().log(LogLevel::INFO, "Servo setup complete.");
}

void ServoMotor::writeServo(double value)
{
    int pwm_value = digital_to_pwm(value, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, 0, 180);
    servo.write(pwm_value);
}
