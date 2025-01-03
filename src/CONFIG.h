#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file CONFIG.h
 * @brief Configurazioni software per il controllo di volo.
 *
 * Questo file contiene le macro che definiscono i parametri configurabili del software,
 * inclusi i valori PWM, i limiti dei servomotori e le soglie per il controllo degli input utente.
 */

/* ========================
 *   PARAMETRI SOFTWARE
 * ======================== */

/**
 * @def PWM_MIN
 * @brief Valore minimo per i segnali PWM.
 */
#define PWM_MIN 1000

/**
 * @def PWM_MAX
 * @brief Valore massimo per i segnali PWM.
 */
#define PWM_MAX 2000

/**
 * @def SERVO_MIN
 * @brief Valore minimo per la mappatura dei servomotori.
 */
#define SERVO_MIN 0

/**
 * @def SERVO_MAX
 * @brief Valore massimo per la mappatura dei servomotori.
 */
#define SERVO_MAX 180

/**
 * @def SERVO_REDUCTION_FACTOR
 * @brief Fattore di riduzione per i servomotori in base alla velocità.
 */
#define SERVO_REDUCTION_FACTOR 0.5

/**
 * @def SWITCH_MIN
 * @brief Valore minimo per la mappatura degli interruttori.
 */
#define SWITCH_MIN 0

/**
 * @def SWITCH_SW_ABC_MAX
 * @brief Valore massimo per la mappatura degli interruttori SW A, B e C.
 */
#define SWITCH_SW_ABC_MAX 2

/**
 * @def SWITCH_SW_D_MAX
 * @brief Valore massimo per la mappatura dell'interruttore SW D.
 */
#define SWITCH_SW_D_MAX 3

/**
 * @def VRA_MIN
 * @brief Valore minimo per il potenziometro A (VRA).
 */
#define VRA_MIN 0

/**
 * @def VRA_MAX
 * @brief Valore massimo per il potenziometro A (VRA).
 */
#define VRA_MAX 100

/**
 * @def VRB_MIN
 * @brief Valore minimo per il potenziometro B (VRB).
 */
#define VRB_MIN 0

/**
 * @def VRB_MAX
 * @brief Valore massimo per il potenziometro B (VRB).
 */
#define VRB_MAX 2

/**
 * @def THROTTLE_MIN
 * @brief Valore minimo per il controllo del throttle.
 */
#define THROTTLE_MIN 0

/**
 * @def THROTTLE_MAX
 * @brief Valore massimo per il controllo del throttle.
 */
#define THROTTLE_MAX 100

/**
 * @def ROLL_MAX
 * @brief Valore massimo per il controllo del rollio (in gradi).
 */
#define ROLL_MAX 90

/**
 * @def PITCH_MAX
 * @brief Valore massimo per il controllo del beccheggio (in gradi).
 */
#define PITCH_MAX 90

/**
 * @def YAW_MAX
 * @brief Valore massimo per il controllo dell'imbardata (in gradi).
 */
#define YAW_MAX 90

/**
 * @def ARM_TOLERANCE
 * @brief Tolleranza percentuale per verificare le condizioni di armamento.
 */
#define ARM_TOLERANCE 10

/**
 * @def BLINK_ON
 * @brief Durata (in millisecondi) dello stato acceso per il lampeggio LED.
 */
#define BLINK_ON 500

/**
 * @def BLINK_OFF
 * @brief Durata (in millisecondi) dello stato spento per il lampeggio LED.
 */
#define BLINK_OFF 500

#endif // CONFIG_H
