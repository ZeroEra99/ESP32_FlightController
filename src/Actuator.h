#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "DataStructures.h"

class Actuator {
public:
    Actuator( );
    virtual void setup() = 0;
};


#endif  // ACTUATOR_H
