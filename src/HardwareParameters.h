/**
 * @file HardwareParameters.h
 * @brief Definizioni dei parametri hardware per il sistema di controllo di volo.
 *
 * Questo file contiene le definizioni dei pin hardware utilizzati per i componenti del sistema,
 * inclusi ricevitore, ESC, servomotori e LED.
 */

#ifndef HARDWARE_PARAMETERS_H
#define HARDWARE_PARAMETERS_H

#include <Arduino.h>

/* ========================
 *   PARAMETRI HARDWARE
 * ======================== */

#define IBUS_RX_PIN 16 // Pin RX per il protocollo iBus

/**
 * @def ESC_PIN
 * @brief Pin associato al controller elettronico di velocità (ESC).
 */
#define ESC_PIN 13

/**
 * @def SERVO_PIN_Y
 * @brief Pin associato al servomotore per il controllo del beccheggio (PITCH).
 */
#define SERVO_PIN_Y 4

/**
 * @def SERVO_PIN_X
 * @brief Pin associato al servomotore per il controllo del rollio (ROLL).
 */
#define SERVO_PIN_X 2

/**
 * @def SERVO_PIN_Z
 * @brief Pin associato al servomotore per il controllo dell'imbardata (YAW).
 */
#define SERVO_PIN_Z 15

/**
 * @def LED_PIN_RED
 * @brief Pin associato al LED rosso.
 */
#define LED_PIN_RED 14

/**
 * @def LED_PIN_GREEN
 * @brief Pin associato al LED verde.
 */
#define LED_PIN_GREEN 27

/**
 * @def LED_PIN_RGB_RED
 * @brief Pin associato al canale rosso del diodo RGB
 */
#define LED_PIN_RGB_RED 26

/**
 * @def LED_PIN_RGB_GREEN
 * @brief Pin associato al canale verde del diodo RGB
 */
#define LED_PIN_RGB_GREEN 25

/**
 * @def LED_PIN_RGB_BLUE
 * @brief Pin associato al canale blu del diodo RGB
 */
#define LED_PIN_RGB_BLUE 33


#endif // HARDWARE_PARAMETERS_H
