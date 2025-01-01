#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "DataStructures.h"
#include "bno055.h"
#include "Motor.h"
#include "ESC.h"

class Aircraft
{
private:
  BNO055 imu;
  Motor servos[3];
  ESC throttle;

public:
  Aircraft();
  FlightData read_imu();

  FlightData flight_data;
};

#endif // AIRCRAFT_H
