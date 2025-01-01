#include "Motor.h"

Motor::Motor(int pin, int analog_min, int analog_max) : Actuator(pin, analog_min, analog_max)
{
}

void Motor::setup()
{
    // Setup del servomotore
    Serial.print("Motor ");Serial.print(pin);Serial.print(" setup starting.\n");
    actuator.attach(pin);
    Serial.print("Motor ");Serial.print(pin);Serial.print(" setup complete.\n");
}

void Motor::write(int value)
{
    // Scrittura del servomotore
    actuator.write(value);
}