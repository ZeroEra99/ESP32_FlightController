#ifndef CONFIG_H
#define CONFIG_H

/* Parametri HARDWARE */
// Definizione dei pin per il ricevitore IA6B
#define IA6B_CHANNELS 6
#define IA6B_PIN_CHANNEL_1 34
#define IA6B_PIN_CHANNEL_2 35
#define IA6B_PIN_CHANNEL_3 36
#define IA6B_PIN_CHANNEL_4 39
#define IA6B_PIN_CHANNEL_5 32
#define IA6B_PIN_CHANNEL_6 33
// Definizione dei pin per l'ESC
#define ESC_PIN 2
// Definizione dei pin per i servomotori
#define SERVO_PIN_PITCH 4
#define SERVO_PIN_ROLL 12
#define SERVO_PIN_YAW 15
// Definizione dei pin per i LED
#define LED_PIN_RED 13
#define LED_PIN_GREEN 14
#define LED_PIN_BLUE 27

/* Parametri SOFTWARE */
#define PWM_MIN 1000
#define PWM_MAX 2000
#define SERVO_MIN 0
#define SERVO_MAX 180

#define BLINK_ON 500
#define BLINK_OFF 500
// Input utente
#define THROTTLE_PILOT_MIN 0
#define THROTTLE_PILOT_MAX 100
#define ROLL_PILOT_MAX 45
#define PITCH_PILOT_MAX 45
#define YAW_PILOT_MAX 45
#define ARM_TOLERANCE 10

// Definizione dei valori di default per il PID
#define KP_ANGLE_ROLL 1
#define KI_ANGLE_ROLL 0
#define KD_ANGLE_ROLL 0
#define KP_ANGLE_PITCH 1
#define KI_ANGLE_PITCH 0
#define KD_ANGLE_PITCH 0
#define KP_ANGLE_YAW 1
#define KI_ANGLE_YAW 0
#define KD_ANGLE_YAW 0
#define MAX_INTEGRAL_ANGLE 0
#define KP_ANGULAR_VELOCITY_ROLL 1
#define KI_ANGULAR_VELOCITY_ROLL 0
#define KD_ANGULAR_VELOCITY_ROLL 0
#define KP_ANGULAR_VELOCITY_PITCH 1
#define KI_ANGULAR_VELOCITY_PITCH 0
#define KD_ANGULAR_VELOCITY_PITCH 0
#define KP_ANGULAR_VELOCITY_YAW 1
#define KI_ANGULAR_VELOCITY_YAW 0
#define KD_ANGULAR_VELOCITY_YAW 0
#define MAX_INTEGRAL_ANGULAR_VELOCITY 0
// Definizione dei valori PID massimi
#define PID_TUNING_MAX_VALUE 2

// Definizione dei parametri del Flight Controller
#define FORWARD_SPEED_THRESHOLD 0.1 // Velocità minima per considerare il drone in movimento veloce


#define QUATERNION_DIM 4
#define EULER_DIM 3

#endif  // CONFIG_H