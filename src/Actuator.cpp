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

Actuator::Actuator(int pin, int pwm_min, int pwm_max, int pwm_null, double digital_min, double digital_max) : pin(pin)
{
    Logger::getInstance().log(LogLevel::INFO, "Actuator setup started.");

    this->pwm_min = pwm_min;
    this->pwm_max = pwm_max;
    this->pwm_null = pwm_null;
    this->digital_min = digital_min;
    this->digital_max = digital_max;

    actuator.attach(pin);
    actuator.writeMicroseconds(pwm_null);

    Logger::getInstance().log(LogLevel::INFO, "Actuator setup complete.");
}

void Actuator::write(double value)
{
    int pwm_value = digital_to_pwm(value, digital_min, digital_max, pwm_min, pwm_max);
    actuator.writeMicroseconds(pwm_value);
}