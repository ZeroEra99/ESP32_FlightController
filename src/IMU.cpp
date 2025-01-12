#include "IMU.h"
//#include "DebugLogger.h"
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
    //DebugLogger::getInstance()->log("IMU setup starting.", LogLevel::DEBUG);
    if (!bno055.begin())
    {
        Serial.println("IMU setup failed!");
        //DebugLogger::getInstance()->log("IMU setup failed!", LogLevel::ERROR);
        //while (1)
            ; // Entra in loop infinito se l'inizializzazione fallisce.
    }
    bno055.setExtCrystalUse(true);
    Serial.println("IMU setup complete.");
    //DebugLogger::getInstance()->log("IMU setup complete.", LogLevel::DEBUG);
}

void IMU::logData(const ImuData &data)
{
    /*
    DebugLogger *logger = DebugLogger::getInstance();

    // Gyroscope
    logger->log("G_X", LogLevel::DATA, false);
    logger->log(data.gyro.x, LogLevel::DATA, false);
    logger->log("G_Y", LogLevel::DATA, false);
    logger->log(data.gyro.y, LogLevel::DATA, false);
    logger->log("G_Z", LogLevel::DATA, false);
    logger->log(data.gyro.z, LogLevel::DATA);

    // Accelerometer
    logger->log("Ac_X", LogLevel::DATA, false);
    logger->log(data.accel.x, LogLevel::DATA, false);
    logger->log("Ac_Y", LogLevel::DATA, false);
    logger->log(data.accel.y, LogLevel::DATA, false);
    logger->log("Ac_Z", LogLevel::DATA, false);
    logger->log(data.accel.z, LogLevel::DATA, false);

    // Quaternion
    logger->log("Q_W", LogLevel::DATA, false);
    logger->log(data.quat.w, LogLevel::DATA, false);
    logger->log("Q_X", LogLevel::DATA, false);
    logger->log(data.quat.x, LogLevel::DATA, false);
    logger->log("Q_Y", LogLevel::DATA, false);
    logger->log(data.quat.y, LogLevel::DATA, false);
    logger->log("Q_Z", LogLevel::DATA, false);
    logger->log(data.quat.z, LogLevel::DATA, false);

    // Linear velocity
    logger->log("V", LogLevel::DATA, false);
    logger->log(data.vel, LogLevel::DATA, false);*/
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
