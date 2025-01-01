#ifndef BNO055_H
#define BNO055_H

#include "DataStructures.h"

class BNO055 {
private:
  void setup();
  // Dati
public:
  BNO055();
  FlightData read();
};

#endif  // BNO055_H
