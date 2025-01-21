#include "IMU.h"
#include "Logger.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Arduino.h>

// Inizializzazione dell'oggetto sensore BNO055
Adafruit_BNO055 bno055 = Adafruit_BNO055(55, 0x28, &Wire);

uint16_t BNO055_SAMPLERATE_DELAY_MS = 10;                                          ///< Frequenza di campionamento in millisecondi.
const double ACCEL_VEL_TRANSITION = (double)(BNO055_SAMPLERATE_DELAY_MS) / 1000.0; ///< Fattore per velocità da accelerazione.
const double ACCEL_POS_TRANSITION = 0.5 * ACCEL_VEL_TRANSITION * ACCEL_VEL_TRANSITION;
const double DEG_2_RAD = 0.01745329251; ///< Conversione da gradi a radianti.

IMU::IMU()
{
    Serial.println("IMU setup starting.");
    Logger::getInstance().log(LogLevel::INFO, "IMU setup starting.");

    if (!bno055.begin())
    {
        Logger::getInstance().log(LogLevel::ERROR, "IMU setup failed!");

        // while (1)
        //; // Entra in loop infinito se l'inizializzazione fallisce.
        Logger::getInstance().log(LogLevel::WARNING, "Continuing without IMU.");
        return;
    }
    bno055.setExtCrystalUse(true);

    isSetupComplete = true;
    Logger::getInstance().log(LogLevel::INFO, "IMU setup complete.");
}

void IMU::logData(const ImuData &data)
{
    Logger::getInstance().logData("G_X", data.gyro.x);
    Logger::getInstance().logData("G_Y", data.gyro.y);
    Logger::getInstance().logData("G_Z", data.gyro.z);

    Logger::getInstance().logData("Ac_X", data.accel.x);
    Logger::getInstance().logData("Ac_Y", data.accel.y);
    Logger::getInstance().logData("Ac_Z", data.accel.z);

    Logger::getInstance().logData("Q_W", data.quat.w);
    Logger::getInstance().logData("Q_X", data.quat.x);
    Logger::getInstance().logData("Q_Y", data.quat.y);
    Logger::getInstance().logData("Q_Z", data.quat.z);

    Logger::getInstance().logData("V", data.vel);
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

    // Log dei dati
    return true;
}
