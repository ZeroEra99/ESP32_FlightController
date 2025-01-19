/**
 * @file Aircraft.h
 * @brief Dichiarazione della classe Aircraft per la gestione dei componenti hardware dell'aereo.
 */

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "DataStructures.h"
#include "IMU.h"
#include "Receiver.h"
#include "LED.h"
#include "Actuator.h"

/**
 * @brief Classe principale per la gestione dell'aereo.
 *
 * Incapsula i componenti hardware principali (ESC, motori, LED, IMU, ricevitore)
 * e fornisce metodi per la lettura dei sensori e il controllo degli attuatori.
 */
class Aircraft
{
private:
    
    Receiver receiver;                    ///< Ricevitore per i comandi del pilota.
    LED led_red, led_green;               ///< LED per il feedback visivo dello stato del sistema.
    RGB_LED led_rgb;                      ///< LED RGB per il feedback visivo dello stato del sistema.
    ESC esc;                              ///< Controller elettronico per i motori.
    ServoMotor servo_x, servo_y, servo_z; ///< Servomotori per il controllo delle superfici di controllo.

public:
    /**
     * @brief Costruttore della classe Aircraft.
     *
     * Inizializza tutti i componenti hardware associati.
     */
    Aircraft();

    /**
     * @brief Legge i dati dall'IMU.
     *
     * Aggiorna la struttura `ImuData` e rileva eventuali errori.
     *
     * @param error Riferimento alla struttura degli errori per aggiornare lo stato dell'IMU.
     */
    void read_imu(Errors &error);

    /**
     * @brief Legge i dati dal ricevitore.
     *
     * Aggiorna la struttura `ReceiverData` e rileva eventuali errori.
     *
     * @param error Riferimento alla struttura degli errori per aggiornare lo stato del ricevitore.
     * @param state Stato attuale del controller per la gestione dei failsafe.
     */
    void read_receiver(Errors &error, CONTROLLER_STATE state);

    /**
     * @brief Aggiorna lo stato dei LED in base allo stato del sistema.
     *
     * @param assist_mode Modalità di assistenza attuale.
     * @param state Stato attuale del controller.
     */
    void update_leds(ASSIST_MODE assist_mode, CONTROLLER_STATE state);

    /**
     * @brief Scrive i valori sugli attuatori.
     *
     * Utilizza i dati di output per controllare i servocomandi e l'ESC.
     */
    void write_actuators();

    IMU imu;
    ImuData imu_data;           ///< Dati letti dall'IMU.
    ReceiverData receiver_data; ///< Dati ricevuti dal pilota.
    Output output;              ///< Output per i servocomandi e l'ESC.
};

#endif // AIRCRAFT_H
