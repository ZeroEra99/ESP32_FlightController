#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "DataStructures.h"
#include "bno055.h"
#include "Motor.h"
#include "ESC.h"

class Aircraft
{
public:
  BNO055 imu;
  Motor servos[3];
  ESC throttle;

public:
  Aircraft();
  FlightData read_imu();
};

#endif // AIRCRAFT_H
