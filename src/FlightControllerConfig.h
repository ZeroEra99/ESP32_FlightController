/**
 * @file FlightControllerConfig.h
 * @brief Configurazioni e parametri per il controllo di volo.
 *
 * Questo file contiene le definizioni dei parametri PID, dei limiti,
 * e delle soglie utilizzati per il controllo del sistema di volo.
 */

#ifndef FLIGHT_CONTROLLER_CONFIG_H
#define FLIGHT_CONTROLLER_CONFIG_H

/* ========================
 *   PARAMETRI PID ANGOLI
 * ======================== */
/**
 * @def KP_ATTITUDE_X
 * @brief Guadagno proporzionale per il controllo dell'angolo di rollio (ROLL).
 */
#define KP_ATTITUDE_X 0

/**
 * @def KI_ATTITUDE_X
 * @brief Guadagno integrale per il controllo dell'angolo di rollio (ROLL).
 */
#define KI_ATTITUDE_X 0

/**
 * @def KD_ATTITUDE_X
 * @brief Guadagno derivativo per il controllo dell'angolo di rollio (ROLL).
 */
#define KD_ATTITUDE_X 0

/**
 * @def KP_ATTITUDE_Y
 * @brief Guadagno proporzionale per il controllo dell'angolo di beccheggio (PITCH).
 */
#define KP_ATTITUDE_Y 0

/**
 * @def KI_ATTITUDE_Y
 * @brief Guadagno integrale per il controllo dell'angolo di beccheggio (PITCH).
 */
#define KI_ATTITUDE_Y 0

/**
 * @def KD_ATTITUDE_Y
 * @brief Guadagno derivativo per il controllo dell'angolo di beccheggio (PITCH).
 */
#define KD_ATTITUDE_Y 0

/**
 * @def KP_ATTITUDE_Z
 * @brief Guadagno proporzionale per il controllo dell'angolo di imbardata (YAW).
 */
#define KP_ATTITUDE_Z 0

/**
 * @def KI_ATTITUDE_Z
 * @brief Guadagno integrale per il controllo dell'angolo di imbardata (YAW).
 */
#define KI_ATTITUDE_Z 0

/**
 * @def KD_ATTITUDE_Z
 * @brief Guadagno derivativo per il controllo dell'angolo di imbardata (YAW).
 */
#define KD_ATTITUDE_Z 0

/* ========================
 *   PARAMETRI PID VELOCITÀ ANGOLARI
 * ======================== */
/**
 * @def KP_GYRO_X
 * @brief Guadagno proporzionale per il controllo della velocità angolare sull'asse X (ROLL).
 */
#define KP_GYRO_X 0

/**
 * @def KI_GYRO_X
 * @brief Guadagno integrale per il controllo della velocità angolare sull'asse X (ROLL).
 */
#define KI_GYRO_X 0

/**
 * @def KD_GYRO_X
 * @brief Guadagno derivativo per il controllo della velocità angolare sull'asse X (ROLL).
 */
#define KD_GYRO_X 0

/**
 * @def KP_GYRO_Y
 * @brief Guadagno proporzionale per il controllo della velocità angolare sull'asse Y (PITCH).
 */
#define KP_GYRO_Y 0

/**
 * @def KI_GYRO_Y
 * @brief Guadagno integrale per il controllo della velocità angolare sull'asse Y (PITCH).
 */
#define KI_GYRO_Y 0

/**
 * @def KD_GYRO_Y
 * @brief Guadagno derivativo per il controllo della velocità angolare sull'asse Y (PITCH).
 */
#define KD_GYRO_Y 0

/**
 * @def KP_GYRO_Z
 * @brief Guadagno proporzionale per il controllo della velocità angolare sull'asse Z (YAW).
 */
#define KP_GYRO_Z 0

/**
 * @def KI_GYRO_Z
 * @brief Guadagno integrale per il controllo della velocità angolare sull'asse Z (YAW).
 */
#define KI_GYRO_Z 0

/**
 * @def KD_GYRO_Z
 * @brief Guadagno derivativo per il controllo della velocità angolare sull'asse Z (YAW).
 */
#define KD_GYRO_Z 0

/* ========================
 *   LIMITI PID
 * ======================== */
/**
 * @def MAX_INTEGRAL_ATTITUDE
 * @brief Limite massimo per il termine integrale del controllo degli angoli.
 */
#define MAX_INTEGRAL_ATTITUDE 0

/**
 * @def MAX_INTEGRAL_GYRO
 * @brief Limite massimo per il termine integrale del controllo delle velocità angolari.
 */
#define MAX_INTEGRAL_GYRO 0

/**
 * @def MAX_INTEGRAL_ANGLE
 * @brief Limite massimo per il termine integrale del controllo dell'angolo.
 */
#define MAX_INTEGRAL_ANGLE 0

/**
 * @def PID_MAX_OFFSET
 * @brief Valore massimo per il tuning dei parametri PID.
 */
#define PID_MAX_OFFSET 2

/**
 * @def TUNING_TARGET_AXIS
 * @brief Asse target per il tuning dei parametri PID.
 */
#define TUNING_TARGET_AXIS 0

/* ========================
 *   PARAMETRI DI VOLO
 * ======================== */
/**
 * @def AUTO_LAND_X
 * @brief Angolo di rollio (ROLL) per l'atterraggio automatico.
 */
#define AUTO_LAND_X 0

/**
 * @def AUTO_LAND_Y
 * @brief Angolo di beccheggio (PITCH) per l'atterraggio automatico.
 */
#define AUTO_LAND_Y 6

/**
 * @def AUTO_LAND_THROTTLE
 * @brief Valore di throttle per l'atterraggio automatico.
 */
#define AUTO_LAND_THROTTLE 0

/**
 * @def AUTO_LAND_Z
 * @brief Angolo di imbardata (YAW) per l'atterraggio automatico.
 */
#define AUTO_LAND_Z 0

/**
 * @def FORWARD_SPEED_THRESHOLD
 * @brief Soglia di velocità in avanti per la riduzione del controllo.
 */
#define FORWARD_SPEED_THRESHOLD 1

#endif // FLIGHT_CONTROLLER_CONFIG_H
