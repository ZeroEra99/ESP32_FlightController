#ifndef IA6B_H
#define IA6B_H

#include "DataStructures.h"
#include "PWM.hpp"

class IA6B {
private:


public:
    IA6B();  // Costruttore che accetta i pin
    void ia6b_setup();
    void ia6b_read();
};

#endif // IA6B_H
