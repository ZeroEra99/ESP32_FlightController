#include "bno055.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Arduino.h>

Adafruit_BNO055 bno055 = Adafruit_BNO055(55, 0x29);  // Oggetto per il sensore BNO055
uint16_t BNO055_SAMPLERATE_DELAY_MS = 10;  // Frequenza di campionamento
// Costante per la transizione da accelerazione a velocità
const double ACCEL_VEL_TRANSITION =  (double)(BNO055_SAMPLERATE_DELAY_MS) / 1000.0;

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
  sensors_event_t linearAccelData;
  bno055.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);

  // Creazione della struttura FlightData
  static double xVel = 0, yVel = 0, zVel = 0; // Velocità integrate
  FlightData data;

  data.angles[X] = euler.x();
  data.angles[Y] = euler.y();
  data.angles[Z] = euler.z();

  data.angular_velocities[X] = angular_velocities.x();
  data.angular_velocities[Y] = angular_velocities.y();
  data.angular_velocities[Z] = angular_velocities.z();

  data.quaternion[W] = quaternion.w();
  data.quaternion[X] = quaternion.x();
  data.quaternion[Y] = quaternion.y();
  data.quaternion[Z] = quaternion.z();

  // Calcola velocità e accelerazione
  data.acceleration[X] = linearAccelData.acceleration.x;
  data.acceleration[Y] = linearAccelData.acceleration.y;
  data.acceleration[Z] = linearAccelData.acceleration.z;

  xVel += ACCEL_VEL_TRANSITION * data.acceleration[X];
  yVel += ACCEL_VEL_TRANSITION * data.acceleration[Y];
  zVel += ACCEL_VEL_TRANSITION * data.acceleration[Z];

  data.velocity[X] = xVel;
  data.velocity[Y] = yVel;
  data.velocity[Z] = zVel;

  return data;
}

