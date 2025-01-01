#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "DataStructures.h"
#include "Receiver.h"
#include "bno055.h"
#include "Motor.h"
#include "ESC.h"

class Aircraft
{
private:
  Receiver receiver;
  BNO055 imu;
  Motor servos[3];
  ESC throttle;

  FlightData flight_data;
  PilotData pilot_data;

public:
  Aircraft();
  void collect_data();
};

#endif // AIRCRAFT_H
