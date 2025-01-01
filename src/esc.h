#ifndef ESC_H
#define ESC_H

#include "Actuator.h"

class ESC : public Actuator {
public:
    ESC( );  // Costruttore che accetta il pin
    void setup() override;  // Implementazione del setup dell'ESC
};

#endif // ESC_H
