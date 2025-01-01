#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "DataStructures.h"
#include <ESP32Servo.h>


class Actuator
{
protected:
  int pin;

public:
  Actuator(int pin);
  virtual void write(int value) = 0;
  
};

#endif // ACTUATOR_H
