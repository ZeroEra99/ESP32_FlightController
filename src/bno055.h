#ifndef BNO_055_H
#define BNO_055_H

#include "DataStructures.h"

class BNO055 {
private:
  void setup();
  // Dati
public:
  BNO055();
  FlightData read();
};

#endif  // BNO_055_H
