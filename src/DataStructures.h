/**
 * @file DataStructures.h
 * @brief Definizione di costanti, enumerazioni e strutture dati utilizzate nel progetto.
 *
 * Questo file contiene dichiarazioni di costanti, enumerazioni e strutture dati
 * necessarie per la gestione dello stato del sistema e dei suoi componenti.
 */

#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "CONFIG.h"
#include "HardwareParameters.h"

/* ========================
 *   COSTANTI
 * ======================== */
/**
 * @brief Dimensione di un quaternione.
 */
#define QUATERNION_DIM 4

/**
 * @brief Dimensione delle coordinate di Eulero.
 */
#define EULER_DIM 3

/* ========================
 *   ENUMERAZIONI
 * ======================== */
/**
 * @brief Identificatori per i componenti di un quaternione.
 */
enum Axis
{
  W, ///< Componente scalare del quaternione.
  X, ///< Componente X del quaternione.
  Y, ///< Componente Y del quaternione.
  Z  ///< Componente Z del quaternione.
};

/**
 * @brief Tipi di input dal pilota.
 */
enum PilotInput
{
  ROLL,
  PITCH,
  THROTTLE,
  YAW,
  SWA,
  SWB,
  SWC,
  SWD,
  VRA,
  VRB
};

/**
 * @brief Indici per i parametri PID.
 */
enum PID
{
  KP, ///< Guadagno proporzionale.
  KI, ///< Guadagno integrale.
  KD  ///< Guadagno derivativo.
};

/**
 * @brief Colori disponibili per i LED.
 */
enum class Color
{
  RED,   ///< Rosso.
  GREEN, ///< Verde.
  BLUE   ///< Blu.
};

/**
 * @brief Stati del LED.
 */
enum class LightState
{
  ON,   ///< LED acceso.
  OFF,  ///< LED spento.
  BLINK ///< LED lampeggiante.
};

/**
 * @brief Stati del sistema.
 */
enum class STATE
{
  FAILSAFE = -1, ///< Modalità di sicurezza.
  DISARMED = 0,  ///< Sistema disarmato.
  ARMED = 1,     ///< Sistema armato.
  AUTO_LAND = 2  ///< Modalità di atterraggio automatico.
};

/**
 * @brief Modalità del controller.
 */
enum class CONTROLLER_MODE
{
  ERROR = -1,     ///< Modalità errore.
  STANDARD = 0,   ///< Modalità standard.
  KP_CALIBRATION, ///< Calibrazione del guadagno proporzionale.
  KI_CALIBRATION, ///< Calibrazione del guadagno integrale.
  KD_CALIBRATION  ///< Calibrazione del guadagno derivativo.
};

/**
 * @brief Obiettivi di calibrazione.
 */
enum class CALIBRATION_TARGET
{
  X, ///< Asse X.
  Y, ///< Asse Y.
  Z  ///< Asse Z.
};

/**
 * @brief Modalità di assistenza.
 */
enum class ASSIST_MODE
{
  MANUAL = 0,          ///< Modalità manuale.
  GYRO_STABILIZED = 1, ///< Stabilizzazione tramite giroscopio.
  ATTITUDE_CONTROL = 2 ///< Controllo dell'attitudine.
};

/* ========================
 *   STRUTTURE DATI
 * ======================== */
/**
 * @struct FlightData
 * @brief Struttura per memorizzare i dati di volo.
 *
 * Contiene informazioni su velocità, accelerazione, giroscopio, attitudine e posizione.
 */
struct FlightData
{
  float velocity[EULER_DIM];      ///< Velocità lineare.
  float acceleration[EULER_DIM];  ///< Accelerazione lineare.
  float gyro[EULER_DIM];          ///< Velocità angolare.
  float attitude[QUATERNION_DIM]; ///< Attitudine rappresentata come quaternione.
  float position[EULER_DIM];      ///< Posizione nello spazio.
};

/**
 * @struct ControllerData
 * @brief Struttura per memorizzare i dati del controller.
 *
 * Contiene input dell'utente, setpoint, errori PID e output per gli attuatori.
 */
struct ControllerData
{
  float user_input[IA6B_CHANNELS];         ///< Input ricevuti dal pilota.
  float setpoint_attitude[QUATERNION_DIM]; ///< Setpoint dell'attitudine.

  float acceleration[EULER_DIM];  ///< Accelerazione lineare misurata.
  float velocity[EULER_DIM];      ///< Velocità lineare misurata.
  float gyro[EULER_DIM];          ///< Velocità angolare misurata.
  float attitude[QUATERNION_DIM]; ///< Attitudine misurata.

  float error_gyro[EULER_DIM];                 ///< Errori del giroscopio per il PID.
  float error_attitude[QUATERNION_DIM];        ///< Errori dell'attitudine per il PID.
  float pid_tuning_offset_gyro[EULER_DIM];     ///< Offset di tuning per il PID del giroscopio.
  float pid_tuning_offset_attitude[EULER_DIM]; ///< Offset di tuning per il PID dell'attitudine.

  float servo_output[EULER_DIM]; ///< Output per i servomotori.
  float esc_output;              ///< Output per l'ESC.
};

#endif // DATA_STRUCTURES_H
