#include "bno055.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Arduino.h>

Adafruit_BNO055 bno055 = Adafruit_BNO055(55, 0x29);  // Oggetto per il sensore BNO055

BNO055::BNO055() {
}

void BNO055::setup() {
  // Configurazione della IMU
  Serial.print("IMU setup starting.\n");
  if (!bno055.begin()) {
    Serial.print("IMU setup failed.\n");
    while (1)
      ;  // Entra in loop infinito se l'inizializzazione fallisce
  }
  Serial.print("IMU setup complete.\n");
}

FlightData BNO055::read() {
  // Leggi i dati dalla IMU
  imu::Vector<3> euler = bno055.getVector(Adafruit_BNO055::VECTOR_EULER);
  imu::Vector<3> angular_velocities = bno055.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  imu::Quaternion quaternion = bno055.getQuat();

  // Creazione della struttura FlightData
  FlightData data;
  data.angles[X] = euler.x();
  data.angles[Y] = euler.y();
  data.angles[Z] = euler.z();
  data.angular_velocities[X] = angular_velocities.x();
  data.angular_velocities[Y] = angular_velocities.y();
  data.angular_velocities[Z] = angular_velocities.z();
  data.quaternion[X] = quaternion.x();
  data.quaternion[Y] = quaternion.y();
  data.quaternion[Z] = quaternion.z();
  data.quaternion[W] = quaternion.w();

  return data;
}
