#ifndef MOTOR_H
#define MOTOR_H

#include "Actuator.h"

class Motor : public Actuator
{
    Servo servo;
    int angle_min, angle_max;

public:
    Motor(int pin, int angle_min, int angle_max);  // Costruttore che accetta il pin
    void write(int value) override;  // Implementazione della scrittura del servomotore
};





#endif // SERVO_H