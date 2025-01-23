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

Actuator::Actuator(int pin)
    : pin(pin)
{
    Logger::getInstance().log(LogLevel::INFO, "Actuator setup started.");
}

ESC::ESC(int pin)
    : Actuator(pin)
{
    actuator.attach(pin, PWM_MIN, PWM_MAX);
    actuator.writeMicroseconds(PWM_MIN);
    Logger::getInstance().log(LogLevel::INFO, "ESC setup complete.");
}

void ESC::write(double value)
{
    int pwm_value = digital_to_pwm(value, THROTTLE_MIN, THROTTLE_MAX, PWM_MIN, PWM_MAX);
    actuator.writeMicroseconds(pwm_value);
}

ServoMotor::ServoMotor(int pin) : Actuator(pin)
{
    actuator.attach(pin);

    int mid_pwm = digital_to_pwm(0, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, PWM_MIN_SERVO, PWM_MAX_SERVO);
    actuator.write(mid_pwm);

    Logger::getInstance().log(LogLevel::INFO, "Servo setup complete.");
}

void ServoMotor::write(double value)
{
    int pwm_value = digital_to_pwm(value, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, PWM_MIN_SERVO, PWM_MAX_SERVO);
    actuator.write(pwm_value);
}
