/**
 * @file Light.cpp
 * @brief Implementazione della classe Light.
 *
 * Questo file contiene l'implementazione delle funzioni della classe Light,
 * utilizzata per controllare un LED con diverse modalità, tra cui accensione,
 * spegnimento e lampeggio.
 */

#include "Light.h"

/**
 * @brief Costruttore della classe Light.
 *
 * Inizializza un LED associato a un determinato pin hardware, specificando
 * il colore e impostandolo nello stato OFF iniziale.
 *
 * @param pin Il pin hardware a cui il LED è collegato.
 * @param color Il colore del LED.
 */
Light::Light(int pin, Color color) : Actuator(pin)
{
    Serial.print("Light ");
    Serial.print(pin);
    Serial.print(" setup starting.\n");

    this->color = color;
    state = LightState::OFF;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);

    Serial.print("Light ");
    Serial.print(pin);
    Serial.print(" setup complete.\n");
}

/**
 * @brief Imposta lo stato del LED (ON o OFF).
 *
 * Cambia lo stato del LED a ON o OFF. Se viene richiesto il lampeggio,
 * la funzione non esegue alcuna modifica.
 *
 * @param state Stato desiderato del LED (ON, OFF).
 */
void Light::set_state(LightState state)
{
    if (state == LightState::BLINK)
        return;

    if (Light::state != state)
        Light::state = state;
}

/**
 * @brief Imposta il lampeggio del LED.
 *
 * Configura i tempi di accensione e spegnimento per il lampeggio
 * del LED e imposta lo stato a BLINK.
 *
 * @param blink_on Durata del periodo ON (in millisecondi).
 * @param blink_off Durata del periodo OFF (in millisecondi).
 */
void Light::set_state(int blink_on, int blink_off)
{
    Light::state = LightState::BLINK;
    Light::blink_on = blink_on;
    Light::blink_off = blink_off;
}

/**
 * @brief Aggiorna lo stato del LED.
 *
 * Controlla e aggiorna il comportamento del LED in base al suo stato attuale.
 * Per il lampeggio, utilizza un approccio non bloccante basato su millis().
 */
void Light::update()
{
    static unsigned long lastUpdate = 0; ///< Memorizza l'ultimo momento in cui il LED è stato aggiornato.
    static bool isOn = false;            ///< Stato corrente del LED durante il lampeggio.

    if (state != LightState::BLINK)
    {
        digitalWrite(pin, state == LightState::ON ? HIGH : LOW);
        return;
    }

    // Lampeggio non bloccante
    unsigned long currentMillis = millis();
    unsigned long millisPassed = currentMillis - lastUpdate;

    if (isOn && millisPassed >= (unsigned long)blink_on)
    {
        // Se il LED è acceso e il tempo "on" è trascorso
        digitalWrite(pin, LOW);
        isOn = false;
        lastUpdate = currentMillis;
    }
    else if (!isOn && millisPassed >= (unsigned long)blink_off)
    {
        // Se il LED è spento e il tempo "off" è trascorso
        digitalWrite(pin, HIGH);
        isOn = true;
        lastUpdate = currentMillis;
    }
}
