/**
 * @file FlightController.h
 * @brief Dichiarazioni per la classe FlightController.
 *
 * La classe FlightController gestisce il controllo di volo, tra cui:
 * - Elaborazione degli input del pilota.
 * - Stabilizzazione del velivolo tramite algoritmi PID.
 * - Lettura dei dati dai sensori.
 * - Calcolo e invio degli output verso gli attuatori.
 */

#ifndef FLIGHT_CONTROLLER_H
#define FLIGHT_CONTROLLER_H

#include "PIDcontrol.h"
#include "DataStructures.h"
#include "FlightControllerConfig.h"

/**
 * @class FlightController
 * @brief Classe che gestisce il controllo di volo.
 *
 * Questa classe implementa i seguenti compiti principali:
 * - Stabilizzazione del velivolo tramite PID su rollio, beccheggio e imbardata.
 * - Controllo delle velocità angolari tramite PID.
 * - Calcolo degli errori di attitudine e velocità.
 * - Tuning dinamico dei parametri PID.
 */
class FlightController
{
private:
    /**
     * @brief Calcola gli offset dinamici per il tuning dei PID.
     *
     * Questo metodo calcola gli offset dinamici per il tuning dei PID in base
     * alla modalità operativa del controller e all'asse target per la calibrazione.
     *
     * @param assist_mode Tipo di assistenza attiva.
     * @param controller_mode Modalità operativa del controller.
     */
    void compute_pid_offset(ASSIST_MODE assist_mode, CONTROLLER_MODE controller_mode, PilotData &pilot_data);

    /**
     * @brief Calcola i dati di controllo in base alle modalità operative e all'input.
     *
     * Questo metodo elabora i dati di volo attuali, gli input del pilota e lo stato
     * del sistema per determinare gli errori e i valori di controllo da utilizzare
     * nei PID.
     *
     * @param dt Intervallo di tempo dall'ultimo aggiornamento (in secondi).
     * @param pilot_data Input del pilota ricevuti dal sistema.
     * @param flight_data Dati di volo attuali, inclusi velocità e orientamento.
     * @param digital_output Output digitali calcolati per gli attuatori.
     * @param assist_mode Modalità di assistenza attuale (manuale, stabilizzato, ecc.).
     * @param state Stato attuale del sistema.
     * @param error Tipo di errore rilevato (se presente).
     * @param controller_mode Modalità operativa del controller (standard, tuning, ecc.).
     */
    void compute_data(double dt, PilotData &pilot_data, FlightData &flight_data, DigitalOutput &digital_output, ASSIST_MODE assist_mode, STATE state, Errors error, CONTROLLER_MODE controller_mode);

    // Componenti logiche
    PIDcontrol pid_attitude_x; ///< PID per il controllo dell'assetto sull'asse X (rollio).
    PIDcontrol pid_attitude_y; ///< PID per il controllo dell'assetto sull'asse Y (beccheggio).
    PIDcontrol pid_attitude_z; ///< PID per il controllo dell'assetto sull'asse Z (imbardata).
    PIDcontrol pid_gyro_x;     ///< PID per il controllo della velocità angolare sull'asse X (rollio).
    PIDcontrol pid_gyro_y;     ///< PID per il controllo della velocità angolare sull'asse Y (beccheggio).
    PIDcontrol pid_gyro_z;     ///< PID per il controllo della velocità angolare sull'asse Z (imbardata).

    // Dati controller di volo
    Euler error_gyro;               ///< Errore delle velocità angolari per ciascun asse (X, Y, Z).
    Quaternion error_attitude;      ///< Errore di attitudine calcolato come differenza tra setpoint e attitudine attuale.
    Quaternion desired_attitude;    ///< Attitudine desiderata calcolata dagli input del pilota.
    PID pid_tuning_offset_gyro;     ///< Offset dinamici per il tuning del PID delle velocità angolari.
    PID pid_tuning_offset_attitude; ///< Offset dinamici per il tuning del PID degli assetti.

    Errors error;

public:
    /**
     * @brief Costruttore della classe FlightController.
     *
     * Inizializza le componenti PID e i parametri necessari per il controllo di volo.
     *
     * @param pilot_data Struttura contenente i dati di input del pilota.
     * @param flight_data Struttura contenente i dati di volo attuali.
     * @param digital_output Struttura per salvare gli output calcolati dal controller.
     */
    FlightController(PilotData &pilot_data, FlightData &flight_data, DigitalOutput &digital_output);

    /**
     * @brief Esegue il controllo di volo.
     *
     * Utilizza i dati attuali per calcolare i segnali di controllo PID
     * e aggiorna gli output digitali per gli attuatori.
     *
     * @param dt Intervallo di tempo dall'ultimo aggiornamento (in secondi).
     * @param flight_data Dati di volo attuali, inclusi velocità e orientamento.
     * @param pilot_data Input del pilota ricevuti dal sistema.
     * @param digital_output Output digitali calcolati per gli attuatori.
     * @param assist_mode Modalità di assistenza attuale (manuale, stabilizzato, ecc.).
     * @param state Stato attuale del sistema.
     * @param calibration_target Asse target per la calibrazione PID (X, Y, Z).
     */
    void control(double dt, FlightData &flight_data, PilotData &pilot_data, DigitalOutput &digital_output, ASSIST_MODE assist_mode, STATE state, CALIBRATION_TARGET calibration_target);
};

#endif // FLIGHT_CONTROLLER_H
