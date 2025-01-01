#include "bno055.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno055 = Adafruit_BNO055(55, 0x29);  // Oggetto per il sensore BNO055

BNO055::BNO055() {
  bno055_setup();
}

void BNO055::bno055_setup() {
  // Configurazione della IMU
  Serial.print("IMU setup starting.\n");
  if (!bno055.begin()) {
    Serial.print("IMU setup failed.\n");
    while (1)
      ;  // Entra in loop infinito se l'inizializzazione fallisce
  }
}

void BNO055::bno055_read() {
  
}
