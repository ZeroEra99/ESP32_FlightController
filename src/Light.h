#ifndef LIGHT_H
#define LIGHT_H

#include "Actuator.h"

class Light : public Actuator
{
private:
    Color color;
    LightState state;
    int blink_on, blink_off;

public:
    Light(int pin, Color color);
    void set_state(LightState state);
    void set_state(int blink_on, int blink_off);
    void update();
};

#endif // LIGHT_H