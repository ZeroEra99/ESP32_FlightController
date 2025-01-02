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

Adafruit_BNO055 bno055 = Adafruit_BNO055(55, 0x29); ///< Oggetto per il sensore BNO055
uint16_t BNO055_SAMPLERATE_DELAY_MS = 10; ///< Frequenza di campionamento in millisecondi
const double ACCEL_VEL_TRANSITION = (double)(BNO055_SAMPLERATE_DELAY_MS) / 1000.0; ///< Costante per la transizione da accelerazione a velocità

/**
 * @brief Costruttore della classe BNO055.
 * 
 * Inizializza l'oggetto senza configurare direttamente il sensore.
 * La configurazione è gestita dal metodo `setup`.
 */
BNO055::BNO055() {
}

/**
 * @brief Configura il sensore BNO055.
 * 
 * Inizializza il sensore, verificando la sua disponibilità e configurandolo.
 * Se l'inizializzazione fallisce, il programma entra in un loop infinito.
 */
void BNO055::setup() {
    Serial.print("IMU setup starting.\n");
    if (!bno055.begin()) {
        Serial.print("IMU setup failed.\n");
        while (1); // Entra in loop infinito se l'inizializzazione fallisce
    }
    Serial.print("IMU setup complete.\n");
}

/**
 * @brief Legge i dati dal sensore BNO055.
 * 
 * Recupera i dati di velocità angolare, accelerazione lineare e orientamento
 * dal sensore, integrando l'accelerazione per calcolare la velocità.
 * 
 * @return Una struttura `FlightData` contenente i dati del sensore.
 */
FlightData BNO055::read() {
    // Leggi i dati dalla IMU
    imu::Vector<3> angular_velocities = bno055.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Quaternion quaternion = bno055.getQuat();
    sensors_event_t linearAccelData;
    bno055.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);

    // Creazione della struttura FlightData
    static double xVel = 0, yVel = 0, zVel = 0; ///< Velocità integrate
    FlightData data;

    data.gyro[X] = angular_velocities.x();
    data.gyro[Y] = angular_velocities.y();
    data.gyro[Z] = angular_velocities.z();

    data.attitude[W] = quaternion.w();
    data.attitude[X] = quaternion.x();
    data.attitude[Y] = quaternion.y();
    data.attitude[Z] = quaternion.z();

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
