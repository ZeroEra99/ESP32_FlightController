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

/* ========================
 *   ENUMERAZIONI
 * ======================== */

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
 * @struct ReceiverPins
 * @brief Struttura per memorizzare i pin del ricevitore.
 *
 * I pin rappresentano i canali fisici utilizzati per leggere i segnali
 * dal ricevitore RC.
 */
struct ReceiverPins{
  int x; ///< Pin per il rollio.
  int y; ///< Pin per il beccheggio.
  int throttle; ///< Pin per il throttle.
  int z; ///< Pin per lo yaw.
  int swa; ///< Pin per lo switch A.
  int swb; ///< Pin per lo switch B.
  int swc; ///< Pin per lo switch C.
  int swd; ///< Pin per lo switch D.
  int vra; ///< Pin per il potenziometro A.
  int vrb; ///< Pin per il potenziometro B.
};

/**
 * @struct Euler
 * @brief Struttura per memorizzare tre componenti vettoriali di tipo float.
 *
 * Utilizzata per rappresentare velocità angolari, accelerazioni o altre grandezze vettoriali.
 */
struct Euler
{
  float x; ///< Componente X.
  float y; ///< Componente Y.
  float z; ///< Componente Z.
};

/**
 * @struct Quaternion
 * @brief Struttura per memorizzare un quaternione.
 *
 * Utilizzato per rappresentare orientamenti e rotazioni nello spazio tridimensionale.
 */
struct Quaternion
{
  float w; ///< Componente scalare.
  float x; ///< Componente X.
  float y; ///< Componente Y.
  float z; ///< Componente Z.
};

/**
 * @struct PID
 * @brief Struttura per memorizzare i parametri di un controllore PID.
 */
struct PID{
  float kp; ///< Guadagno proporzionale.
  float ki; ///< Guadagno integrale.
  float kd; ///< Guadagno derivativo.
  float max_integral; ///< Limite massimo per l'integrale.
};

/**
 * @struct AnalogOutput
 * @brief Struttura per memorizzare i valori analogici di output.
 */
struct AnalogOutput{
  int x; ///< Output per l'asse X.
  int y; ///< Output per l'asse Y.
  int z; ///< Output per l'asse Z.
  int throttle; ///< Output per il throttle.
};

/**
 * @struct DigitalOutput
 * @brief Struttura per memorizzare i valori digitali di output.
 */
struct DigitalOutput{
  float x; ///< Output per l'asse X.
  float y; ///< Output per l'asse Y.
  float z; ///< Output per l'asse Z.
  float throttle; ///< Output per il throttle.
};

/**
 * @struct FlightData
 * @brief Struttura per memorizzare i dati di volo.
 *
 * Contiene informazioni su:
 * - Velocità lineare.
 * - Accelerazione lineare.
 * - Velocità angolare (giroscopio).
 * - Attitudine (orientamento sotto forma di quaternione).
 */
struct FlightData
{
  Euler velocity;    ///< Velocità lineare.
  Euler acceleration; ///< Accelerazione lineare.
  Euler gyro;        ///< Velocità angolare.
  Quaternion attitude; ///< Attitudine.
};

/**
 * @struct PilotData
 * @brief Struttura per memorizzare i dati di input dell'utente.
 *
 * Rappresenta i valori letti dai canali del ricevitore RC per il controllo del sistema.
 */
struct PilotData{
  float x; ///< Input per il rollio.
  float y; ///< Input per il beccheggio.
  float throttle; ///< Input per il throttle.
  float z; ///< Input per lo yaw.
  float swa; ///< Input per lo switch A.
  float swb; ///< Input per lo switch B.
  float swc; ///< Input per lo switch C.
  float swd; ///< Input per lo switch D.
  float vra; ///< Input per il potenziometro A.
  float vrb; ///< Input per il potenziometro B.
};

#endif // DATA_STRUCTURES_H
