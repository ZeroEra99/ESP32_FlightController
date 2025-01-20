/**
 * @file LED.h
 * @brief Dichiarazione della classe LED per la gestione dei LED semplici e RGB.
 */

#ifndef LED_H
#define LED_H

#include "DataStructures.h"

/**
 * @brief Classe per la gestione dei LED.
 *
 * Supporta la gestione dello stato e del lampeggio per un LED.
 */
class LED
{
private:
    LED_STATE state; ///< Stato attuale del LED (ON, OFF, BLINK).
    int blink_on;    ///< Durata del periodo ON durante il lampeggio (in millisecondi).
    int blink_off;   ///< Durata del periodo OFF durante il lampeggio (in millisecondi).
    int pin;         ///< Pin associato al LED.

public:
    /**
     * @brief Costruttore della classe LED.
     *
     * Inizializza il LED con lo stato OFF.
     *
     * @param pin Pin associato al LED.
     */
    explicit LED(int pin);

    /**
     * @brief Imposta lo stato del LED.
     *
     * @param state Nuovo stato del LED.
     *
     * @note Se lo stato è OFF, il LED si spegnerà.
     * @note Se lo stato è ON, il LED si accenderà.
     */
    void set_state(LED_STATE state);

    /**
     * @brief Imposta lo stato del LED in modalità lampeggio.
     *
     * @param blink_on Durata del periodo ON durante il lampeggio (in millisecondi).
     * @param blink_off Durata del periodo OFF durante il lampeggio (in millisecondi).
     */
    void set_state(int blink_on, int blink_off);

    /**
     * @brief Aggiorna lo stato del LED.
     *
     * Gestisce il lampeggio del LED in base al tempo.
     */
    void update();
};

/**
 * @brief Classe per la gestione dei LED RGB.
 *
 * Supporta la gestione del colore e del lampeggio per un LED RGB.
 */
class RGB_LED
{
private:
    int pin_red;   ///< Pin per il LED rosso.
    int pin_green; ///< Pin per il LED verde.
    int pin_blue;  ///< Pin per il LED blu.

    int channel_red;   ///< Canale PWM per il LED rosso.
    int channel_green; ///< Canale PWM per il LED verde.
    int channel_blue;  ///< Canale PWM per il LED blu.

    LED_STATE state; ///< Stato attuale del LED (ON, OFF, BLINK).
public:
    /**
     * @brief Costruttore della classe RGB_LED
     *
     * @param pin_red Pin fisico per il rosso
     * @param channel_red Canale PWM per il rosso
     * @param pin_green Pin fisico per il verde
     * @param channel_green Canale PWM per il verde
     * @param pin_blue Pin fisico per il blu
     * @param channel_blue Canale PWM per il blu
     * @param freq Frequenza PWM (default: 5000 Hz)
     * @param resolution Risoluzione PWM (default: 8 bit)
     */
    RGB_LED(int pin_red, int channel_red, int pin_green, int channel_green, int pin_blue, int channel_blue, int freq = 5000, int resolution = 8);

    /**
     * @brief Imposta lo stato del LED RGB.
     *
     * @param state Nuovo stato del LED.
     *
     * @note Se lo stato è OFF, il LED si spegnerà.
     * @note Se lo stato è ON, il LED si accenderà.
     */
    void set_state(LED_STATE state);

    /**
     * @brief Imposta lo stato del LED RGB.
     *
     * @param state Nuovo stato del LED.
     * @param color Nuovo colore del LED.
     *
     * @note Se lo stato è OFF, il LED si spegnerà.
     * @note Se lo stato è ON, il LED si accenderà.
     */
    void set_state(LED_STATE state, COLOR color);

    /**
     * @brief Imposta lo stato del LED RGB in modalità lampeggio.
     *
     * @param blink_on Durata del periodo ON durante il lampeggio (in millisecondi).
     * @param blink_off Durata del periodo OFF durante il lampeggio (in millisecondi).
     * @param color Colore del LED.
     */
    void set_state(int blink_on, int blink_off, COLOR color);

    /**
     * @brief Scrive il colore del LED RGB.
     *
     * @param state Stato del LED.
     * @param color Colore del LED.
     */
    void write(bool state, COLOR color);

    /**
     * @brief Aggiorna lo stato del LED RGB.
     *
     * Gestisce il lampeggio del LED in base al tempo.
     */
    void update();

    int blink_on;  ///< Durata del periodo ON durante il lampeggio (in millisecondi).
    int blink_off; ///< Durata del periodo OFF durante il lampeggio (in millisecondi).
    COLOR color;   ///< Colore del LED.
};

#endif // LED_H
