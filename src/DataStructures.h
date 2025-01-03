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
 * @brief Tipologie di errore.
 */
enum class ERROR_TYPE
{
  NONE, ///< Nessun errore.

  PILOT_ERROR_AXIS, ///< Errore sull'asse del pilota.
  PILOT_ERROR_SW,   ///< Errore sugli switch del pilota.

  IMU_ERROR_QUATERNION, ///< Errore sul quaternione dell'IMU.
  IMU_ERROR_GYRO,       ///< Errore sul giroscopio dell'IMU.
  IMU_ERROR_ACCEL,      ///< Errore sull'accelerometro dell'IMU.

  CONTROLLER_ERROR ///< Errore sul controllore.
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
  MANUAL = 0,           ///< Modalità manuale.
  GYRO_STABILIZED = 1,  ///< Stabilizzazione tramite giroscopio.
  ATTITUDE_CONTROL = 2, ///< Controllo dell'attitudine.
};

/* ========================
 *   STRUTTURE DATI
 * ======================== */
/**
 * @struct Euler
 * @brief Struttura per memorizzare tre componenti vettoriali di tipo float.
 *
 * Utilizzata per rappresentare velocità angolari, accelerazioni o altre grandezze vettoriali.
 */
struct Euler
{
  float x; ///< Componente X (rollio).
  float y; ///< Componente Y (beccheggio).
  float z; ///< Componente Z (imbardata).
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
  float x; ///< Componente X (i).
  float y; ///< Componente Y (j).
  float z; ///< Componente Z (k).
};

/**
 * @struct PID
 * @brief Struttura per memorizzare i parametri di un controllore PID.
 */
struct PID
{
  float kp;           ///< Guadagno proporzionale.
  float ki;           ///< Guadagno integrale.
  float kd;           ///< Guadagno derivativo.
  float max_integral; ///< Limite massimo per l'integrale.
};

/**
 * @struct AnalogOutput
 * @brief Struttura per memorizzare i valori analogici di output.
 */
struct AnalogOutput
{
  int x;        ///< Output per l'asse X.
  int y;        ///< Output per l'asse Y.
  int z;        ///< Output per l'asse Z.
  int throttle; ///< Output per il throttle.
};

/**
 * @struct DigitalOutput
 * @brief Struttura per memorizzare i valori digitali di output.
 */
struct DigitalOutput
{
  float x;        ///< Output per l'asse X.
  float y;        ///< Output per l'asse Y.
  float z;        ///< Output per l'asse Z.
  float throttle; ///< Output per il throttle.
};

/**
 * @struct FlightData
 * @brief Struttura per memorizzare i dati di volo.
 *
 * Contiene informazioni su:
 * - Velocità lineare (calcolata).
 * - Posizione (calcolata).
 * - Accelerazione lineare.
 * - Velocità angolare (giroscopio).
 * - Attitudine (orientamento sotto forma di quaternione).
 */
struct FlightData
{
  float forward_speed; ///< Velocità lineare in avanti.
  Euler velocity;      ///< Velocità lineare.
  Euler position;      ///< Posizione.
  Euler acceleration;  ///< Accelerazione lineare.
  Euler gyro;          ///< Velocità angolare.
  Quaternion attitude; ///< Attitudine.
};

/**
 * @struct PilotData
 * @brief Struttura per memorizzare i dati di input dell'utente.
 *
 * Rappresenta i valori al ricevitore RC mappati in segnali digitali.
 */
struct PilotData
{
  int16_t x;        ///< Input per il rollio.
  int16_t y;        ///< Input per il beccheggio.
  int16_t throttle; ///< Input per il throttle.
  int16_t z;        ///< Input per lo yaw.
  int16_t swa;      ///< Input per lo switch A.
  int16_t swb;      ///< Input per lo switch B.
  int16_t swc;      ///< Input per lo switch C.
  int16_t swd;      ///< Input per lo switch D.
  int16_t vra;      ///< Input per il potenziometro A.
  int16_t vrb;      ///< Input per il potenziometro B.
};

/**
 * @struct PilotDataAnalog
 * @brief Struttura per memorizzare i dati di input dell'utente in formato analogico.
 *
 * Rappresenta i valori letti dai canali del ricevitore RC per il controllo del sistema.
 */
struct PilotDataAnalog
{
  int x;        ///< Input per il rollio analogico.
  int y;        ///< Input per il beccheggio analogico.
  int throttle; ///< Input per il throttle analogico.
  int z;        ///< Input per lo yaw analogico.
  int swa;      ///< Input per lo switch A analogico.
  int swb;      ///< Input per lo switch B analogico.
  int swc;      ///< Input per lo switch C analogico.
  int swd;      ///< Input per lo switch D analogico.
  int vra;      ///< Input per il potenziometro A analogico.
  int vrb;      ///< Input per il potenziometro B analogico.
};

#endif // DATA_STRUCTURES_H
