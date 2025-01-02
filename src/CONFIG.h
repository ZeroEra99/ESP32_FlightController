#ifndef CONFIG_H
#define CONFIG_H

/* ========================
 *   PARAMETRI SOFTWARE
 * ======================== */
// Valori PWM
#define PWM_MIN 1000
#define PWM_MAX 2000

// Range dei servomotori
#define SERVO_MIN 0
#define SERVO_MAX 180

// Input utente
#define THROTTLE_MIN 0
#define THROTTLE_MAX 100
#define ROLL_MAX 45
#define PITCH_MAX 45
#define YAW_MAX 45

// Tolleranza per armamento
#define ARM_TOLERANCE 10

// Blink LED
#define BLINK_ON 500
#define BLINK_OFF 500

#endif // CONFIG_H
