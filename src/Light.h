/**
 * @file Light.h
 * @brief Dichiarazioni per la classe Light.
 *
 * La classe Light rappresenta un attuatore per gestire un LED con diverse modalità,
 * incluse accensione, spegnimento e lampeggio.
 */

#ifndef LIGHT_H
#define LIGHT_H

#include "Actuator.h"

/**
 * @class Light
 * @brief Implementazione di un attuatore per un LED.
 *
 * Questa classe estende Actuator e consente di controllare un LED con diverse
 * modalità di funzionamento, tra cui accensione, spegnimento e lampeggio.
 */
class Light : public Actuator
{
private:
    Color color;      ///< Colore del LED (può essere usato per gestire LED RGB).
    LightState state; ///< Stato attuale del LED (ON, OFF, BLINK).
    int blink_on;     ///< Durata del periodo ON durante il lampeggio (in millisecondi).
    int blink_off;    ///< Durata del periodo OFF durante il lampeggio (in millisecondi).

public:
    /**
     * @brief Costruttore della classe Light.
     *
     * Inizializza un LED associato a un determinato pin hardware e specifica il colore.
     *
     * @param pin Il pin hardware a cui il LED è collegato.
     * @param color Il colore del LED.
     */
    Light(int pin, Color color);

    /**
     * @brief Imposta lo stato del LED.
     *
     * Consente di accendere, spegnere o impostare il LED per lampeggiare.
     *
     * @param state Stato desiderato del LED (ON, OFF, BLINK).
     */
    void set_state(LightState state);

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
     * @brief Aggiorna lo stato del LED.
     *
     * Controlla e aggiorna il comportamento del LED (ad esempio, gestisce il lampeggio).
     */
    void update();
};

#endif // LIGHT_H
