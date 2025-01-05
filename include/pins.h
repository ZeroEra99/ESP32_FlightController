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
#define IBUS_RX_PIN 16 ///< Pin RX per il protocollo iBus.

#define ESC_PIN 13 ///< Pin per il controller elettronico ESC.

#define SERVO_PIN_Y 4 ///< Pin per il servocomando sull'asse Y.
#define SERVO_PIN_X 2 ///< Pin per il servocomando sull'asse X.
#define SERVO_PIN_Z 15 ///< Pin per il servocomando sull'asse Z.

#define LED_PIN_RED 14 ///< Pin per il LED rosso.
#define LED_PIN_GREEN 27 ///< Pin per il LED verde.

#define LED_PIN_RGB_RED 26   ///< Pin per il canale rosso del LED RGB.
#define LED_PIN_RGB_GREEN 25 ///< Pin per il canale verde del LED RGB.
#define LED_PIN_RGB_BLUE 33  ///< Pin per il canale blu del LED RGB.
/** @} */

#endif // PIN_H
