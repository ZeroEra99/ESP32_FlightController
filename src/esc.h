#ifndef ESC_H
#define ESC_H

#include "Actuator.h"


class ESC : public Actuator {
private:
    Servo esc;
    int pwm_min,pwn_max;
public:
    ESC(int pin, int analog_min, int analog_max);  // Costruttore che accetta il pin
    void write(int value) override;  // Implementazione della scrittura dell'ESC
};

#endif // ESC_H
