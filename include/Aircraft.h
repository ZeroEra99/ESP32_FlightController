/**
 * @file Aircraft.h
 * @brief Dichiarazione della classe Aircraft per la gestione dei componenti hardware dell'aereo.
 */

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "DataStructures.h"
#include "IMU.h"
#include "Receiver.h"
#include "esc.h"
#include "ServoMotor.h"
#include "LED.h"

/**
 * @brief Classe principale per la gestione dell'aereo.
 * 
 * Incapsula i componenti hardware principali (ESC, motori, LED, IMU, ricevitore)
 * e fornisce metodi per la lettura dei sensori e il controllo degli attuatori.
 */
class Aircraft
{
private:
    ESC esc;                    ///< Controller elettronico per il motore principale.
    ServoMotor servo_x, servo_y, servo_z; ///< Servocomandi per gli assi X, Y e Z.
    Receiver receiver;          ///< Ricevitore per i comandi del pilota.
    IMU bno055;                 ///< Sensore IMU (BNO055) per il controllo dell'assetto.
    LED led_green, led_red, led_rgb; ///< LED per il feedback visivo dello stato del sistema.

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
     */
    void read_receiver(Errors &error);

    /**
     * @brief Aggiorna lo stato dei LED in base allo stato del sistema.
     * 
     * @param assist_mode Modalità di assistenza corrente.
     * @param state Stato attuale del controller.
     * @param error Riferimento alla struttura degli errori.
     */
    void update_leds(ASSIST_MODE assist_mode, CONTROLLER_STATE state, Errors error);

    /**
     * @brief Scrive i valori sugli attuatori.
     * 
     * Utilizza i dati di output per controllare i servocomandi e l'ESC.
     */
    void write_actuators();

    ImuData imu_data;           ///< Dati letti dall'IMU.
    ReceiverData receiver_data; ///< Dati ricevuti dal pilota.
    Output output;              ///< Output per gli attuatori.
};

#endif // AIRCRAFT_H
