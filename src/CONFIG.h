#ifndef CONFIG_H
#define CONFIG_H

/* Parametri HARDWARE */
// Definizione dei pin per il ricevitore IA6B
#define IA6B_PIN_CHANNEL_1 34
#define IA6B_PIN_CHANNEL_2 35
#define IA6B_PIN_CHANNEL_3 36
#define IA6B_PIN_CHANNEL_4 39
// Definizione dei pin per l'ESC
#define ESC_PIN 2
// Definizione dei pin per i servomotori
#define SERVO_PIN_PITCH 4
#define SERVO_PIN_ROLL 12
#define SERVO_PIN_YAW 15

#define IA6B_CHANNELS 4

#define PWM_MIN 1000
#define PWM_MAX 2000
#define SERVO_MIN 0
#define SERVO_MAX 180


#endif  // CONFIG_H