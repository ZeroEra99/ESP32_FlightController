#ifndef BNO055_H
#define BNO055_H

#include "DataStructures.h"

class BNO055 {
private:
  void bno055_setup();
  // Dati
public:
  BNO055();
  void bno055_read();
};

#endif  // BNO055_H
