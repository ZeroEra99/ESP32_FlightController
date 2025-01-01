#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "DataStructures.h"
#include <ESP32Servo.h>


class Actuator
{
protected:
  int pin, analog_min, analog_max;
  Servo actuator;

public:
  Actuator(int pin, int analog_min, int analog_max);
  virtual void setup() = 0;
  virtual void write(int value) = 0;
  
};

#endif // ACTUATOR_H
