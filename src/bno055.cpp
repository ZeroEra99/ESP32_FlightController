/**
 * @file bno055.cpp
 * @brief Implementazione della classe BNO055.
 *
 * Questo file contiene l'implementazione delle funzioni della classe BNO055,
 * che fornisce un'interfaccia per il sensore di orientamento BNO055.
 */

#include "bno055.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Arduino.h>

Adafruit_BNO055 bno055 = Adafruit_BNO055(55, 0x29);                                ///< Oggetto per il sensore BNO055
uint16_t BNO055_SAMPLERATE_DELAY_MS = 10;                                          ///< Frequenza di campionamento in millisecondi
const double ACCEL_VEL_TRANSITION = (double)(BNO055_SAMPLERATE_DELAY_MS) / 1000.0; ///< Costante per la transizione da accelerazione a velocità

/**
 * @brief Costruttore della classe BNO055.
 *
 * Inizializza l'oggetto senza configurare direttamente il sensore.
 * La configurazione deve essere eseguita chiamando il metodo `setup()`.
 */
BNO055::BNO055()
{
}

/**
 * @brief Configura il sensore BNO055.
 *
 * Questo metodo inizializza il sensore, verificando la sua disponibilità e configurandolo
 * per il funzionamento. Se l'inizializzazione fallisce, il programma entra in un loop infinito.
 */
void BNO055::setup()
{
    Serial.print("IMU setup starting.\n");
    if (!bno055.begin())
    {
        Serial.print("IMU setup failed.\n");
        while (1)
            ; // Entra in loop infinito se l'inizializzazione fallisce
    }
    Serial.print("IMU setup complete.\n");
}

/**
 * @brief Legge i dati dal sensore BNO055.
 *
 * Questo metodo legge i dati dal sensore BNO055, inclusi:
 * - Velocità angolari dal giroscopio.
 * - Orientamento sotto forma di quaternione.
 * - Accelerazione lineare.
 *
 * Integra l'accelerazione per calcolare la velocità lineare. I risultati vengono restituiti
 * in una struttura `FlightData`.
 *
 * @return Una struttura `FlightData` contenente i dati letti dal sensore.
 */
FlightData BNO055::read()
{
    // Leggi i dati dalla IMU
    imu::Vector<3> angular_velocities = bno055.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Quaternion quaternion = bno055.getQuat();
    sensors_event_t linearAccelData;
    bno055.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);

    // Creazione della struttura FlightData
    static double xVel = 0, yVel = 0, zVel = 0; ///< Velocità integrate
    FlightData data;

    // Velocità angolari (giroscopio)
    data.gyro.x = angular_velocities.x();
    data.gyro.y = angular_velocities.y();
    data.gyro.z = angular_velocities.z();

    // Orientamento (quaternione)
    data.attitude.w = quaternion.w();
    data.attitude.x = quaternion.x();
    data.attitude.y = quaternion.y();
    data.attitude.z = quaternion.z();

    // Accelerazione lineare
    data.acceleration.x = linearAccelData.acceleration.x;
    data.acceleration.y = linearAccelData.acceleration.y;
    data.acceleration.z = linearAccelData.acceleration.z;

    // Calcolo della velocità integrando l'accelerazione
    xVel += ACCEL_VEL_TRANSITION * data.acceleration.x;
    yVel += ACCEL_VEL_TRANSITION * data.acceleration.y;
    zVel += ACCEL_VEL_TRANSITION * data.acceleration.z;

    data.velocity.x = xVel;
    data.velocity.y = yVel;
    data.velocity.z = zVel;

    return data;
}
