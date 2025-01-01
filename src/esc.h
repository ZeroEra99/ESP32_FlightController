#ifndef ESC_H
#define ESC_H

#include "Actuator.h"

class ESC : public Actuator {
public:
    ESC(int pin, int analog_min, int analog_max);  // Costruttore che accetta il pin
    void setup() override;  // Implementazione del setup dell'ESC
    void write(int value) override;  // Implementazione della scrittura dell'ESC
};

#endif // ESC_H
