#include "Actuator.h"

/**
 * @brief Converte un valore digitale in PWM, rispettando i limiti configurati.
 */
int Actuator::digital_to_pwm(double value, double min_digital, double max_digital, int min_analog, int max_analog)
{
    int pwm_value = (int)((value - min_digital) * (max_analog - min_analog) / (max_digital - min_digital) + min_analog);

    if (pwm_value < min_analog)
    {
        pwm_value = min_analog;
    }
    else if (pwm_value > max_analog)
    {
        pwm_value = max_analog;
    }

    return pwm_value;
}

/**
 * @brief Converte un valore PWM in digitale, rispettando i limiti configurati.
 */
double Actuator::pwm_to_digital(int pwm_value, int min_analog, int max_analog, double min_digital, double max_digital)
{
    double digital_value = (double)(pwm_value - min_analog) / (max_analog - min_analog) * (max_digital - min_digital) + min_digital;

    if (digital_value < min_digital)
    {
        digital_value = min_digital;
    }
    else if (digital_value > max_digital)
    {
        digital_value = max_digital;
    }

    return digital_value;
}
