/**
 * @file pins.h
 * @brief Definizione dei pin associati ai componenti hardware.
 */

#ifndef PIN_H
#define PIN_H

/**
 * @brief Definizione dei pin utilizzati nel sistema.
 *
 * Questo file contiene le definizioni dei pin associati ai vari componenti hardware.
 */

/** @defgroup Pin_Definitions Pin Definiti
 *  @{
 */
#define IBUS_RX_PIN 18 ///< Pin RX per il protocollo iBus.

#define SERVO_PIN_X 10 ///< Pin per il servocomando sull'asse X.
#define SERVO_PIN_Y 11  ///< Pin per il servocomando sull'asse Y.
#define SERVO_PIN_Z 12 ///< Pin per il servocomando sull'asse Z.

#define ESC_PIN 13 ///< Pin per il controller elettronico ESC.

#define LED_PIN_RED 6   ///< Pin per il LED rosso.
#define LED_PIN_GREEN 7 ///< Pin per il LED verde.

#define LED_PIN_RGB_RED 15   ///< Pin per il canale rosso del LED RGB.
#define LED_PIN_RGB_GREEN 16 ///< Pin per il canale verde del LED RGB.
#define LED_PIN_RGB_BLUE 17  ///< Pin per il canale blu del LED RGB.
/** @} */

#endif // PIN_H
