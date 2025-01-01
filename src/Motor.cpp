#include "Motor.h"

Motor::Motor(int pin, int angle_min, int angle_max) : Actuator(pin)
{
    Serial.print("Servo ");
    Serial.print(pin);
    Serial.print(" setup starting.\n");

    angle_min = angle_min;
    angle_max = angle_max;
    servo.attach(pin);
    Serial.print("Servo ");
    Serial.print(pin);
    Serial.print(" setup complete.\n");
}


void Motor::write(int value)
{
    // Scrittura del servomotore
    servo.write(value);
}