#include "IMU.h"
#include "DebugLogger.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Arduino.h>


// Inizializzazione dell'oggetto sensore BNO055
Adafruit_BNO055 bno055 = Adafruit_BNO055(55, 0x29);

uint16_t BNO055_SAMPLERATE_DELAY_MS = 10; ///< Frequenza di campionamento in millisecondi.
const double ACCEL_VEL_TRANSITION = (double)(BNO055_SAMPLERATE_DELAY_MS) / 1000.0; ///< Fattore per velocità da accelerazione.
const double ACCEL_POS_TRANSITION = 0.5 * ACCEL_VEL_TRANSITION * ACCEL_VEL_TRANSITION;
const double DEG_2_RAD = 0.01745329251; ///< Conversione da gradi a radianti.

IMU::IMU()
{
    DebugLogger::getInstance()->log("IMU setup starting.", LogLevel::DEBUG);
    if (!bno055.begin())
    {
        DebugLogger::getInstance()->log("IMU setup failed!", LogLevel::ERROR);
        while (1)
            ; // Entra in loop infinito se l'inizializzazione fallisce.
    }
    DebugLogger::getInstance()->log("IMU setup complete.", LogLevel::DEBUG);
}

bool IMU::read(ImuData &data)
{
    // Legge i dati dai sensori dell'IMU
    imu::Vector<3> angular_velocities = bno055.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Quaternion quaternion = bno055.getQuat();
    sensors_event_t linearAccelData;
    sensors_event_t orientationData;

    if (bno055.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL) || 
        bno055.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER))
    {
        return false;
    }

    // Velocità angolari
    data.gyro.x = angular_velocities.x();
    data.gyro.y = angular_velocities.y();
    data.gyro.z = angular_velocities.z();

    // Orientamento
    data.quat.w = quaternion.w();
    data.quat.x = quaternion.x();
    data.quat.y = quaternion.y();
    data.quat.z = quaternion.z();

    // Accelerazione lineare
    data.accel.x = linearAccelData.acceleration.x;
    data.accel.y = linearAccelData.acceleration.y;
    data.accel.z = linearAccelData.acceleration.z;

    // Velocità lineare
    data.vel = ACCEL_VEL_TRANSITION * linearAccelData.acceleration.x / cos(DEG_2_RAD * orientationData.orientation.x);

    return true;
}
