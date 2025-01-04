/**
 * @file LED.h
 * @brief Dichiarazioni per la classe LED.
 *
 * La classe LED rappresenta un attuatore per gestire un LED con diverse modalità,
 * incluse accensione, spegnimento e lampeggio.
 */

#ifndef LED_H
#define LED_H

#include "Actuator.h"

/**
 * @class LED
 * @brief Implementazione di un attuatore per un LED.
 *
 * Questa classe estende Actuator e consente di controllare un LED con diverse
 * modalità di funzionamento, tra cui accensione, spegnimento e lampeggio.
 */
class LED : public Actuator
{
private:
    Color color;      ///< Colore del LED (può essere usato per gestire LED RGB).
    LightState state; ///< Stato attuale del LED (ON, OFF, BLINK).
    int blink_on;     ///< Durata del periodo ON durante il lampeggio (in millisecondi).
    int blink_off;    ///< Durata del periodo OFF durante il lampeggio (in millisecondi).
    int pin_red;
    int pin_green;
    int pin_blue;

public:
    /**
     * @brief Costruttore della classe LED.
     *
     * Inizializza un LED  semplice associato a un determinato pin hardware,
     * impostandolo nello stato OFF iniziale.
     *
     * @param pin Il pin hardware a cui il LED è collegato.
     */
    LED(int pin);

    /**
     * @brief Costruttore della classe LED.
     *
     * Inizializza un LED RGB associato a tre  pin hardware.
     *
     * @param pin Il pin hardware a cui il LED è collegato.
     */
    LED(int pin_red, int pin_green, int pin_blue);

    /**
     * @brief Imposta lo stato del LED.
     *
     * Consente di accendere, spegnere o impostare il LED per lampeggiare.
     *
     * @param state Stato desiderato del LED (ON, OFF, BLINK).
     */
    void set_state(LightState state);

    /**
     * @brief Imposta lo stato del LED RGB.
     *
     * Consente di accendere, spegnere o impostare il LED per lampeggiare.
     *
     * @param state Stato desiderato del LED (ON, OFF, BLINK).
     * @param color Colore desiderateo del LED.
     */
    void set_state(LightState state, Color color);

    /**
     * @brief Imposta il lampeggio del LED.
     *
     * Specifica la durata dei periodi ON e OFF per il lampeggio.
     *
     * @param blink_on Durata del periodo ON (in millisecondi).
     * @param blink_off Durata del periodo OFF (in millisecondi).
     */
    void set_state(int blink_on, int blink_off);

    /**
     * @brief Imposta il lampeggio del LED.
     *
     * Specifica la durata dei periodi ON e OFF per il lampeggio.
     *
     * @param blink_on Durata del periodo ON (in millisecondi).
     * @param blink_off Durata del periodo OFF (in millisecondi).
     * @param color Colore desiderato del LED.
     */
    void set_state(int blink_on, int blink_off, Color color);

    /**
     * @brief Funzione per accendere un led RGB
     *
     * @param on Stato del LED (true = acceso, false = spento).
     * @param color Colore del LED.
     */
    void write(bool on, Color color);

    /**
     * @brief Aggiorna lo stato del LED.
     *
     * Controlla e aggiorna il comportamento del LED (ad esempio, gestisce il lampeggio).
     */
    void update();
};

#endif // LED_H
