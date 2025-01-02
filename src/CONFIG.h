#ifndef CONFIG_H
#define CONFIG_H

/* ========================
 *   PARAMETRI SOFTWARE
 * ======================== */
// Valori PWM
#define PWM_MIN 1000
#define PWM_MAX 2000
// Range di mappatura dei servomotori
#define SERVO_MIN 0
#define SERVO_MAX 180
// Range di mappatura degli interruttori
#define SWITCH_MIN 0
#define SWITCH_SW_ABC_MAX 2
#define SWITCH_SW_D_MAX 3
// Range di mappatura dei potenziometri
#define VRA_MIN 0
#define VRA_MAX 100
#define VRB_MIN 0
#define VRB_MAX 100
// Parametri input utente
#define THROTTLE_MIN 0
#define THROTTLE_MAX 100
#define ROLL_MAX 90
#define PITCH_MAX 90
#define YAW_MAX 90
// Tolleranza per armamento
#define ARM_TOLERANCE 10

// Blink LED
#define BLINK_ON 500
#define BLINK_OFF 500

#endif // CONFIG_H
