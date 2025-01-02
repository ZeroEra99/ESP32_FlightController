#ifndef FLIGHT_CONTROLLER_CONFIG_H
#define FLIGHT_CONTROLLER_CONFIG_H

/* ========================
 *   PARAMETRI PID ANGOLI
 * ======================== */
// PID per il controllo degli angoli (ROLL, PITCH, YAW)
#define KP_ATTITUDE_ROLL 1  // Guadagno proporzionale per ROLL
#define KI_ATTITUDE_ROLL 0  // Guadagno integrale per ROLL
#define KD_ATTITUDE_ROLL 0  // Guadagno derivativo per ROLL

#define KP_ATTITUDE_PITCH 1  // Guadagno proporzionale per PITCH
#define KI_ATTITUDE_PITCH 0  // Guadagno integrale per PITCH
#define KD_ATTITUDE_PITCH 0  // Guadagno derivativo per PITCH

#define KP_ATTITUDE_YAW 1  // Guadagno proporzionale per YAW
#define KI_ATTITUDE_YAW 0  // Guadagno integrale per YAW
#define KD_ATTITUDE_YAW 0  // Guadagno derivativo per YAW

/* ========================
 *   PARAMETRI PID VELOCITÀ ANGOLARI
 * ======================== */
// PID per il controllo delle velocità angolari (ROLL, PITCH, YAW)
#define KP_GYRO_ROLL 1  // Guadagno proporzionale per ROLL
#define KI_GYRO_ROLL 0  // Guadagno integrale per ROLL
#define KD_GYRO_ROLL 0  // Guadagno derivativo per ROLL

#define KP_GYRO_PITCH 1  // Guadagno proporzionale per PITCH
#define KI_GYRO_PITCH 0  // Guadagno integrale per PITCH
#define KD_GYRO_PITCH 0  // Guadagno derivativo per PITCH

#define KP_GYRO_YAW 1  // Guadagno proporzionale per YAW
#define KI_GYRO_YAW 0  // Guadagno integrale per YAW
#define KD_GYRO_YAW 0  // Guadagno derivativo per YAW

/* ========================
 *   LIMITI PID
 * ======================== */
#define MAX_INTEGRAL_ATTITUDE 0  // Limite massimo per l'integrale del controllo degli angoli (disabilitato)
#define MAX_INTEGRAL_GYRO 0      // Limite massimo per l'integrale delle velocità angolari (disabilitato)
#define MAX_INTEGRAL_ANGLE 0     // Limite massimo per l'integrale del controllo degli angoli (disabilitato)
#define PID_MAX_OFFSET 2         // Valore massimo per il tuning PID

/* ========================
 *   PARAMETRI DI VOLO
 * ======================== */
#define MOVE_SPEED_THRESHOLD 0.1  // Soglia minima per considerare il sistema in movimento
#define MOVE_ACCEL_THRESHOLD 0.1  // Soglia minima per considerare il sistema in accelerazione
#define FORWARD_SPEED_THRESHOLD 0.1  // Soglia minima per considerare il sistema in movimento veloce

#endif // FLIGHT_CONTROLLER_CONFIG_H
