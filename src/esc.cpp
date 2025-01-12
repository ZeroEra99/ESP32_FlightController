#include "ESC.h"
//#include "DebugLogger.h"

ESC::ESC(int pin) : Actuator(pin)
{
    // Configurazione iniziale dell'ESC
    /*
    DebugLogger::getInstance()->log("ESC ", LogLevel::DEBUG);
    DebugLogger::getInstance()->log(pin, LogLevel::DEBUG);
    DebugLogger::getInstance()->log(" setup starting.", LogLevel::DEBUG);
*/
    Serial.println("ESC setup starting.");
    esc.attach(pin);
    esc.writeMicroseconds(PWM_MIN); ///< Imposta il segnale PWM al valore minimo iniziale.
    delay(1000); ///< Attesa per la calibrazione dell'ESC.

    Serial.println("ESC setup complete.");
    //DebugLogger::getInstance()->log("ESC setup complete.", LogLevel::DEBUG);
}

void ESC::write(double value)
{
    // Converte il valore digitale in PWM e lo invia all'ESC
    int analog = digital_to_pwm(value, THROTTLE_MIN, THROTTLE_MAX, PWM_MIN, PWM_MAX);
    esc.writeMicroseconds(analog);
}
