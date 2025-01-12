#include "ServoMotor.h"
//#include "DebugLogger.h"

ServoMotor::ServoMotor(int pin) : Actuator(pin)
{
    // Configurazione del servomotore
    //DebugLogger::getInstance()->log("Servo ", LogLevel::DEBUG);
    //DebugLogger::getInstance()->log(pin, LogLevel::DEBUG);
    //DebugLogger::getInstance()->log(" setup starting.", LogLevel::DEBUG);

    servo.attach(pin);

    Serial.println("Servo setup complete.");

    //DebugLogger::getInstance()->log("Servo ", LogLevel::DEBUG);
    //DebugLogger::getInstance()->log(pin, LogLevel::DEBUG);
    //DebugLogger::getInstance()->log(" setup complete.", LogLevel::DEBUG);
}

void ServoMotor::write(double value)
{
    // Converte il valore digitale in PWM e lo invia al servomotore
    int analog = digital_to_pwm(value, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, SERVO_MIN, SERVO_MAX);
    servo.write(analog);
}
