/**
 * @file LED.h
 * @brief Dichiarazione della classe LED per la gestione dei LED semplici e RGB.
 */

#ifndef LED_H
#define LED_H

#include "Actuator.h"

/**
 * @brief Classe per la gestione dei LED.
 * 
 * Supporta LED semplici e RGB, permettendo di controllare lo stato, il colore e il lampeggio.
 */
class LED : public Actuator
{
private:
    COLOR color;     ///< Colore del LED (può essere usato per gestire LED RGB).
    LED_STATE state; ///< Stato attuale del LED (ON, OFF, BLINK).
    int blink_on;    ///< Durata del periodo ON durante il lampeggio (in millisecondi).
    int blink_off;   ///< Durata del periodo OFF durante il lampeggio (in millisecondi).
    int pin_red;     ///< Pin per il LED rosso (per LED RGB).
    int pin_green;   ///< Pin per il LED verde (per LED RGB).
    int pin_blue;    ///< Pin per il LED blu (per LED RGB).

public:
    /**
     * @brief Costruttore per un LED semplice.
     * 
     * @param pin Pin associato al LED.
     */
    LED(int pin);

    /**
     * @brief Costruttore per un LED RGB.
     * 
     * @param pin_red Pin per il LED rosso.
     * @param pin_green Pin per il LED verde.
     * @param pin_blue Pin per il LED blu.
     */
    LED(int pin_red, int pin_green, int pin_blue);

    /**
     * @brief Imposta lo stato del LED.
     * 
     * @param state Stato desiderato (ON, OFF, BLINK).
     */
    void set_state(LED_STATE state);

    /**
     * @brief Imposta lo stato e il colore del LED.
     * 
     * @param state Stato desiderato (ON, OFF, BLINK).
     * @param color Colore desiderato.
     */
    void set_state(LED_STATE state, COLOR color);

    /**
     * @brief Imposta il lampeggio del LED.
     * 
     * @param blink_on Durata del periodo ON (in millisecondi).
     * @param blink_off Durata del periodo OFF (in millisecondi).
     */
    void set_state(int blink_on, int blink_off);

    /**
     * @brief Imposta il lampeggio e il colore del LED.
     * 
     * @param blink_on Durata del periodo ON (in millisecondi).
     * @param blink_off Durata del periodo OFF (in millisecondi).
     * @param color Colore desiderato.
     */
    void set_state(int blink_on, int blink_off, COLOR color);

    /**
     * @brief Scrive lo stato e il colore del LED.
     * 
     * @param on Stato del LED (true = acceso, false = spento).
     * @param color Colore desiderato.
     */
    void write(bool on, COLOR color);

    /**
     * @brief Aggiorna lo stato del LED.
     * 
     * Deve essere chiamato periodicamente per gestire il lampeggio.
     */
    void update();
};

#endif // LED_H
