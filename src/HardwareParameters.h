/**
 * @file HardwareParameters.h
 * @brief Definizioni dei parametri hardware per il sistema di controllo di volo.
 *
 * Questo file contiene le definizioni dei pin hardware utilizzati per i componenti del sistema,
 * inclusi ricevitore, ESC, servomotori e LED.
 */

#ifndef HARDWARE_PARAMETERS_H
#define HARDWARE_PARAMETERS_H

/* ========================
 *   PARAMETRI HARDWARE
 * ======================== */

/**
 * @def IA6B_CHANNELS
 * @brief Numero totale di canali del ricevitore IA6B.
 */
#define IA6B_CHANNELS 10

/**
 * @def FLIGHT_CHANNELS
 * @brief Numero di canali principali utilizzati per il controllo di volo.
 */
#define FLIGHT_CHANNELS 4

/**
 * @def IA6B_PIN_CHANNEL_1
 * @brief Pin associato al canale 1 (PITCH) del ricevitore IA6B.
 */
#define IA6B_PIN_CHANNEL_1 34

/**
 * @def IA6B_PIN_CHANNEL_2
 * @brief Pin associato al canale 2 (ROLL) del ricevitore IA6B.
 */
#define IA6B_PIN_CHANNEL_2 35

/**
 * @def IA6B_PIN_CHANNEL_3
 * @brief Pin associato al canale 3 (THROTTLE) del ricevitore IA6B.
 */
#define IA6B_PIN_CHANNEL_3 36

/**
 * @def IA6B_PIN_CHANNEL_4
 * @brief Pin associato al canale 4 (YAW) del ricevitore IA6B.
 */
#define IA6B_PIN_CHANNEL_4 39

/**
 * @def IA6B_PIN_CHANNEL_5
 * @brief Pin associato al canale 5 (SWITCH A) del ricevitore IA6B.
 */
#define IA6B_PIN_CHANNEL_5 32

/**
 * @def IA6B_PIN_CHANNEL_6
 * @brief Pin associato al canale 6 (SWITCH B) del ricevitore IA6B.
 */
#define IA6B_PIN_CHANNEL_6 33

/**
 * @def IA6B_PIN_CHANNEL_7
 * @brief Pin associato al canale 7 (SWITCH C) del ricevitore IA6B.
 */
#define IA6B_PIN_CHANNEL_7 25

/**
 * @def IA6B_PIN_CHANNEL_8
 * @brief Pin associato al canale 8 (SWITCH D) del ricevitore IA6B.
 */
#define IA6B_PIN_CHANNEL_8 26

/**
 * @def IA6B_PIN_CHANNEL_9
 * @brief Pin associato al canale 9 (POTENZIOMETRO A) del ricevitore IA6B.
 */
#define IA6B_PIN_CHANNEL_9 27

/**
 * @def IA6B_PIN_CHANNEL_10
 * @brief Pin associato al canale 10 (POTENZIOMETRO B) del ricevitore IA6B.
 */
#define IA6B_PIN_CHANNEL_10 14

/**
 * @def ESC_PIN
 * @brief Pin associato al controller elettronico di velocità (ESC).
 */
#define ESC_PIN 2

/**
 * @def SERVO_PIN_Y
 * @brief Pin associato al servomotore per il controllo del beccheggio (PITCH).
 */
#define SERVO_PIN_Y 4

/**
 * @def SERVO_PIN_X
 * @brief Pin associato al servomotore per il controllo del rollio (ROLL).
 */
#define SERVO_PIN_X 12

/**
 * @def SERVO_PIN_Z
 * @brief Pin associato al servomotore per il controllo dell'imbardata (YAW).
 */
#define SERVO_PIN_Z 15

/**
 * @def LED_PIN_RED
 * @brief Pin associato al LED rosso.
 */
#define LED_PIN_RED 13

/**
 * @def LED_PIN_GREEN
 * @brief Pin associato al LED verde.
 */
#define LED_PIN_GREEN 14

/**
 * @def LED_PIN_BLUE
 * @brief Pin associato al LED blu.
 */
#define LED_PIN_BLUE 27

#endif // HARDWARE_PARAMETERS_H
