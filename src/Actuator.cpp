#include "Actuator.h"
#include "HardwareParameters.h"
#include "Logger.h"

/**
 * @brief Funzione per limitare un valore all'interno di un intervallo.
 */
template <typename T>
constexpr const T& clamp(const T& value, const T& min, const T& max) {
    return (value < min) ? min : (value > max) ? max : value;
}

/**
 * @brief Converte un valore digitale in PWM, rispettando i limiti configurati.
 */
int digital_to_pwm(double value, double min_digital, double max_digital, int min_analog, int max_analog)
{
    // Calcola il valore PWM scalato e limita direttamente il risultato nel range specificato
    return clamp(static_cast<int>(((value - min_digital) * (max_analog - min_analog) / (max_digital - min_digital)) + min_analog), min_analog, max_analog);
}

ESC::ESC(int pin, int pwmChannel, int frequency, int resolution)
    : pin(pin), pwmChannel(pwmChannel), frequency(frequency), resolution(resolution)
{
    // Configura il canale PWM
    ledcSetup(pwmChannel, frequency, resolution);
    // Associa il canale PWM al pin
    ledcAttachPin(pin, pwmChannel);

    // Inizializza il segnale PWM al valore minimo
    int min_pwm = digital_to_pwm(THROTTLE_MIN, THROTTLE_MIN, THROTTLE_MAX, PWM_MIN, PWM_MAX);
    ledcWrite(pwmChannel, min_pwm);

    Logger::getInstance().log(LogLevel::INFO, "ESC setup complete.");
}

void ESC::write(double value)
{
    // Converte il valore digitale in PWM e lo invia al canale
    int pwm_value = digital_to_pwm(value, THROTTLE_MIN, THROTTLE_MAX, PWM_MIN, PWM_MAX);
    ledcWrite(pwmChannel, pwm_value);
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
