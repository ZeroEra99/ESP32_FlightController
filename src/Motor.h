#ifndef MOTOR_H
#define MOTOR_H

#include "Actuator.h"

class Motor : public Actuator
{

public:
    Motor(int pin, int analog_min, int analog_max);  // Costruttore che accetta il pin
    void setup() override;  // Implementazione del setup del servomotore
    void write(int value) override;  // Implementazione della scrittura del servomotore
};





#endif // SERVO_H