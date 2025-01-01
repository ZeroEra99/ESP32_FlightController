#ifndef RECEIVER_H
#define RECEIVER_H

#include "DataStructures.h"
#include "PWM.hpp"

class Receiver {
private:
    int pins[IA6B_CHANNELS];


public:
    Receiver(int *pins);  // Costruttore che accetta i pin
    int* read();
};

#endif // RECEIVER_H
