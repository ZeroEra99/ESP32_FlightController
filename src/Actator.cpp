#include "Actuator.h"
#include "HardwareParameters.h"
#include "Logger.h"

/**
 * @brief Converte un valore digitale in PWM, rispettando i limiti configurati.
 */
int digital_to_pwm(double value, double min_digital, double max_digital, int min_analog, int max_analog)
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

ESC::ESC(int pin)
{
    // Configurazione iniziale dell'ESC
    esc.attach(pin);
    esc.writeMicroseconds(PWM_MIN); ///< Imposta il segnale PWM al valore minimo iniziale.

    Logger::getInstance().log(LogLevel::INFO, "ESC setup complete.");
}

void ESC::write(double value)
{
    // Converte il valore digitale in PWM e lo invia all'ESC
    int analog = digital_to_pwm(value, THROTTLE_MIN, THROTTLE_MAX, PWM_MIN, PWM_MAX);
    esc.writeMicroseconds(analog);
}

ServoMotor::ServoMotor(int pin)
{
    // Configurazione del servomotore
    servo.attach(pin);
    servo.write(0); ///< Imposta il servomotore all'angolo minimo iniziale.
    Logger::getInstance().log(LogLevel::INFO, "Servo setup complete.");
}

void ServoMotor::write(double value)
{
    // Converte il valore digitale in PWM e lo invia al servomotore
    int analog = digital_to_pwm(value, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, SERVO_MIN, SERVO_MAX);
    servo.write(analog);
}
