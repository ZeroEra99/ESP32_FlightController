/**
 * @file FlightControllerConfig.h
 * @brief Configurazioni per i parametri PID e altre impostazioni del controllo di volo.
 */

#ifndef FLIGHT_CONTROLLER_CONFIG_H
#define FLIGHT_CONTROLLER_CONFIG_H

/**
 * @brief Configurazioni del controller di volo.
 * 
 * Questo file contiene le definizioni di macro per i parametri PID, limiti e altri valori di configurazione
 * utilizzati nel sistema.
 */

/** @defgroup PID_Parameters Parametri PID
 *  @{
 */

// Parametri PID per il controllo dell'assetto (attitude)
#define KP_ATTITUDE_X 0 ///< Guadagno proporzionale per l'asse X (rollio).
#define KI_ATTITUDE_X 0 ///< Guadagno integrale per l'asse X (rollio).
#define KD_ATTITUDE_X 0 ///< Guadagno derivativo per l'asse X (rollio).

#define KP_ATTITUDE_Y 0 ///< Guadagno proporzionale per l'asse Y (beccheggio).
#define KI_ATTITUDE_Y 0 ///< Guadagno integrale per l'asse Y (beccheggio).
#define KD_ATTITUDE_Y 0 ///< Guadagno derivativo per l'asse Y (beccheggio).

#define KP_ATTITUDE_Z 0 ///< Guadagno proporzionale per l'asse Z (imbardata).
#define KI_ATTITUDE_Z 0 ///< Guadagno integrale per l'asse Z (imbardata).
#define KD_ATTITUDE_Z 0 ///< Guadagno derivativo per l'asse Z (imbardata).

// Parametri PID per il controllo della velocità angolare (gyro)
#define KP_GYRO_X 0 ///< Guadagno proporzionale per l'asse X (rollio).
#define KI_GYRO_X 0 ///< Guadagno integrale per l'asse X (rollio).
#define KD_GYRO_X 0 ///< Guadagno derivativo per l'asse X (rollio).

#define KP_GYRO_Y 0 ///< Guadagno proporzionale per l'asse Y (beccheggio).
#define KI_GYRO_Y 0 ///< Guadagno integrale per l'asse Y (beccheggio).
#define KD_GYRO_Y 0 ///< Guadagno derivativo per l'asse Y (beccheggio).

#define KP_GYRO_Z 0 ///< Guadagno proporzionale per l'asse Z (imbardata).
#define KI_GYRO_Z 0 ///< Guadagno integrale per l'asse Z (imbardata).
#define KD_GYRO_Z 0 ///< Guadagno derivativo per l'asse Z (imbardata).

/** @} */

/** @defgroup Limits Limiti e soglie
 *  @{
 */

#define MAX_INTEGRAL_ATTITUDE 0 ///< Limite massimo per il valore integrale dell'attitudine.
#define MAX_INTEGRAL_GYRO 0     ///< Limite massimo per il valore integrale del gyro.
#define MAX_INTEGRAL_ANGLE 0    ///< Limite massimo per l'angolo calcolato dai PID.

#define PID_MAX_OFFSET 2 ///< Offset massimo per il tuning PID.

#define TUNING_TARGET_AXIS 0 ///< Asse target per il tuning (0 = X, 1 = Y, 2 = Z).

#define FORWARD_SPEED_THRESHOLD 1 ///< Soglia della velocità in avanti per la riduzione del controllo.
#define SERVO_REDUCTION_FACTOR 0.5 ///< Fattore di riduzione per i servo.

#define AUTO_LAND_X 0        ///< Posizione automatica per il rollio in modalità di atterraggio.
#define AUTO_LAND_Y 6        ///< Posizione automatica per il beccheggio in modalità di atterraggio.
#define AUTO_LAND_THROTTLE 0 ///< Valore del throttle in modalità di atterraggio.
#define AUTO_LAND_Z 0        ///< Posizione automatica per lo yaw in modalità di atterraggio.

/** @} */

#endif // FLIGHT_CONTROLLER_CONFIG_H
