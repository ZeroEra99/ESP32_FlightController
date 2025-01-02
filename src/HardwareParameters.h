#ifndef HARDWARE_PARAMETERS_H
#define HARDWARE_PARAMETERS_H

/* ========================
 *   PARAMETRI HARDWARE
 * ======================== */
// Pin per il ricevitore IA6B - NON ATTUALI
#define IA6B_CHANNELS 10
#define FLIGHT_CHANNELS 4
#define IA6B_PIN_CHANNEL_1 34  // pitch
#define IA6B_PIN_CHANNEL_2 35  // roll
#define IA6B_PIN_CHANNEL_3 36  // throttle
#define IA6B_PIN_CHANNEL_4 39  // yaw
#define IA6B_PIN_CHANNEL_5 32  // switch A
#define IA6B_PIN_CHANNEL_6 33  // switch B
#define IA6B_PIN_CHANNEL_7 25  // switch C
#define IA6B_PIN_CHANNEL_8 26  // switch D
#define IA6B_PIN_CHANNEL_9 27  // potenziometro A
#define IA6B_PIN_CHANNEL_10 14 // potenziometro B

// Pin per l'ESC - NON ATTUALE
#define ESC_PIN 2

// Pin per i servomotori - NON ATTUALI
#define SERVO_PIN_PITCH 4
#define SERVO_PIN_ROLL 12
#define SERVO_PIN_YAW 15

// Pin per i LED - NON ATTUALI
#define LED_PIN_RED 13
#define LED_PIN_GREEN 14
#define LED_PIN_BLUE 27

#endif // HARDWARE_PARAMETERS_H
