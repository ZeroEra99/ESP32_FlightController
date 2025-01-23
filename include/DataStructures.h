/**
 * @file DataStructures.h
 * @brief Definizione delle strutture dati e enumerazioni utilizzate nel sistema.
 */
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "HardwareParameters.h"

/** @defgroup Enumerations Enumerazioni
 *  Enumerazioni utilizzate nel sistema.
 *  @{
 */

/**
 * @brief Enumerazione per i colori disponibili per i LED.
 */
enum class COLOR
{
    RED,        ///< Rosso.
    GREEN,      ///< Verde.
    BLUE,       ///< Blu.
    LIGHT_BLUE, ///< Blu chiaro.
    WHITE,      ///< Bianco.
    PURPLE,     ///< Viola.
    NONE        ///< Nessun colore (spento).
};

/**
 * @brief Enumerazione per gli stati del LED.
 */
enum class LED_STATE
{
    ON,   ///< LED acceso.
    OFF,  ///< LED spento.
    BLINK ///< LED lampeggiante.
};

/**
 * @brief Enumerazione per gli stati del controller.
 */
enum class CONTROLLER_STATE
{
    FAILSAFE = -1, ///< Modalità failsafe.
    DISARMED = 0,  ///< Sistema disarmato.
    ARMED = 1,     ///< Sistema armato.
};

/**
 * @brief Enumerazione per le modalità di controllo.
 */
enum class CONTROLLER_MODE
{
    STANDARD = 0,   ///< Modalità standard.
    KP_CALIBRATION, ///< Calibrazione del guadagno proporzionale.
    KI_CALIBRATION, ///< Calibrazione del guadagno integrale.
    KD_CALIBRATION  ///< Calibrazione del guadagno derivativo.
};

/**
 * @brief Enumerazione per gli assi target della calibrazione PID.
 */
enum class CALIBRATION_TARGET
{
    X, ///< Asse X.
    Y, ///< Asse Y.
    Z  ///< Asse Z.
};

/**
 * @brief Enumerazione per le modalità di assistenza.
 */
enum class ASSIST_MODE
{
    MANUAL = 0,           ///< Modalità manuale.
    GYRO_STABILIZED = 1,  ///< Stabilizzazione tramite giroscopio.
    ATTITUDE_CONTROL = 2, ///< Controllo dell'attitudine.
};

/**
 * @brief Enumerazione per i livelli di log disponibili.
 */
enum class LogLevel
{
    ERROR,   ///< Errore critico.
    WARNING, ///< Avvertimento.
    INFO,    ///< Informazione.
};

/** @} */

/** @defgroup Structures Strutture
 *  Strutture dati utilizzate nel sistema.
 *  @{
 */

/**
 * @struct Euler
 * @brief Rappresenta un vettore Euler (rollio, beccheggio, imbardata).
 */
struct Euler
{
    float x; ///< Componente X (rollio).
    float y; ///< Componente Y (beccheggio).
    float z; ///< Componente Z (imbardata).
};

/**
 * @struct Quaternion
 * @brief Rappresenta un quaternione.
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
 * @brief Contiene i parametri di un PID controller.
 */
struct PID
{
    float kp;           ///< Guadagno proporzionale.
    float ki;           ///< Guadagno integrale.
    float kd;           ///< Guadagno derivativo.
    float max_integral; ///< Limite massimo per l'integrale.
};

/**
 * @struct Output
 * @brief Contiene gli output sui tre assi.
 */
struct Output
{
    float x;        ///< Output per l'asse X.
    float y;        ///< Output per l'asse Y.
    float z;        ///< Output per l'asse Z.
    float throttle; ///< Output per il throttle.
};

/**
 * @struct ImuData
 * @brief Contiene i dati letti dall'IMU.
 */
struct ImuData
{
    Euler gyro;      ///< Velocità angolari (giroscopio).
    Euler accel;     ///< Accelerazioni lineari.
    Quaternion quat; ///< Orientamento (quaternione).
    float vel;       ///< Velocità integrata.
};

/**
 * @struct ReceiverData
 * @brief Contiene i dati ricevuti dal pilota.
 */
struct ReceiverData
{
    float x;        ///< Input per il rollio.
    float y;        ///< Input per il beccheggio.
    float throttle; ///< Input per il throttle.
    float z;        ///< Input per lo yaw.
    float swa;      ///< Input per lo switch A.
    float swb;      ///< Input per lo switch B.
    float swc;      ///< Input per lo switch C.
    float swd;      ///< Input per lo switch D.
    float vra;      ///< Input per il potenziometro A.
    float vrb;      ///< Input per il potenziometro B.
};

/**
 * @struct Errors
 * @brief Rappresenta gli errori di sistema.
 */
struct Errors
{
    bool RECEIVER_ERROR; ///< Errore sul pilota.
    bool IMU_ERROR;      ///< Errore sull'IMU.
};

/** @} */

#endif // DATA_STRUCTURES_H
