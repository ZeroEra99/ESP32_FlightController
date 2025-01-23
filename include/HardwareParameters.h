/**
 * @file HardwareParameters.h
 * @brief Definizione dei limiti e dei parametri hardware del sistema.
 */

#ifndef HARDWARE_PARAMETERS_H
#define HARDWARE_PARAMETERS_H

/**
 * @brief Parametri hardware del sistema.
 *
 * Questo file contiene le definizioni di macro relative ai limiti hardware
 * e ai valori utilizzati per configurare il sistema.
 */

#define ESC_PWM_CHANNEL 0 ///< Canale PWM utilizzato per l'ESC.

#define RGB_RED_PWM_CHANNEL 1   ///< Canale PWM utilizzato per il LED rosso.
#define RGB_GREEN_PWM_CHANNEL 2 ///< Canale PWM utilizzato per il LED verde.
#define RGB_BLUE_PWM_CHANNEL 3  ///< Canale PWM utilizzato per il LED blu.

#define SERVO_X_PWM_CHANNEL 4 ///< Canale PWM utilizzato per il servomotore X.
#define SERVO_Y_PWM_CHANNEL 5 ///< Canale PWM utilizzato per il servomotore Y.
#define SERVO_Z_PWM_CHANNEL 6 ///< Canale PWM utilizzato per il servomotore Z.

/** @defgroup PWM_Ranges Intervalli PWM
 *  @{
 */
#define PWM_MIN 1000 ///< Valore minimo del segnale PWM.
#define PWM_MAX 2000 ///< Valore massimo del segnale PWM.
#define PWM_MIN_SERVO 1000 ///< Valore minimo del segnale PWM per i servomotori.
#define PWM_NEUTRAL_SERVO 1500 ///< Valore neutro del segnale PWM per i servomotori.
#define PWM_MAX_SERVO 2000 ///< Valore massimo del segnale PWM per i servomotori.
/** @} */

/** @defgroup Servo_Ranges Intervalli Servo
 *  @{
 */
#define SERVO_MIN_ANGLE -90 ///< Angolo minimo per il servo.
#define SERVO_MAX_ANGLE 90  ///< Angolo massimo per il servo.
/** @} */

/** @defgroup Switch_Ranges Intervalli Switch
 *  @{
 */
#define SWITCH_MIN 0        ///< Valore minimo per gli switch.
#define SWITCH_SW_ABD_MAX 1 ///< Valore massimo per gli switch A, B e C.
#define SWITCH_SW_C_MAX 2   ///< Valore massimo per lo switch D.
/** @} */

/** @defgroup Potentiometer_Ranges Intervalli Potenziometri
 *  @{
 */
#define VRA_MIN 0   ///< Valore minimo per il potenziometro A.
#define VRA_MAX 100 ///< Valore massimo per il potenziometro A.
#define VRB_MIN 0   ///< Valore minimo per il potenziometro B.
#define VRB_MAX 2   ///< Valore massimo per il potenziometro B.
/** @} */

/** @defgroup Throttle_Ranges Intervalli Throttle
 *  @{
 */
#define THROTTLE_MIN 0   ///< Valore minimo per il throttle.
#define THROTTLE_MAX 100 ///< Valore massimo per il throttle.
/** @} */

/** @defgroup Axis_Ranges Intervalli assi di volo
 *  @{
 */
#define ROLL_MIN -90 ///< Valore minimo per il rollio.
#define ROLL_MAX 90  ///< Valore massimo per il rollio.

#define PITCH_MIN -90 ///< Valore minimo per il beccheggio.
#define PITCH_MAX 90  ///< Valore massimo per il beccheggio.

#define YAW_MIN -90 ///< Valore minimo per lo yaw.
#define YAW_MAX 90  ///< Valore massimo per lo yaw.

/** @defgroup Arm_Disarm_Parameters Parametri Arm/Disarm
 *  @{
 */
#define ARM_TOLERANCE 1 ///< Tolleranza per il comando di arm/disarm.
/** @} */

/** @defgroup LED_Timings Tempi LED
 *  @{
 */
#define BLINK_ON 1000  ///< Durata ON del lampeggio LED (millisecondi).
#define BLINK_OFF 1000 ///< Durata OFF del lampeggio LED (millisecondi).
/** @} */

#endif // HARDWARE_PARAMETERS_H
