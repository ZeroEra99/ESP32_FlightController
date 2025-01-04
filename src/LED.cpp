/**
 * @file LED.cpp
 * @brief Implementazione della classe LED.
 *
 * Questo file contiene l'implementazione delle funzioni della classe LED,
 * utilizzata per controllare un LED con diverse modalità, tra cui accensione,
 * spegnimento e lampeggio.
 */

#include "LED.h"

/**
 * @brief Costruttore della classe LED.
 *
 * Inizializza un LED  semplice associato a un determinato pin hardware,
 * impostandolo nello stato OFF iniziale.
 *
 * @param pin Il pin hardware a cui il LED è collegato.
 */
LED::LED(int pin) : Actuator(pin)
{
    this->pin_red = 0;
    this->pin_green = 0;
    this->pin_blue = 0;
    
    Serial.print("Light ");
    Serial.print(pin);
    Serial.print(" setup starting.\n");

    this->pin = pin;
    this->blink_on = 0;
    this->blink_off = 0;
    state = LightState::OFF;
    color = {};
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);

    Serial.print("Light ");
    Serial.print(pin);
    Serial.print(" setup complete.\n");
}

/**
 * @brief Costruttore della classe LED.
 *
 * Inizializza un LED RGB associato a tre  pin hardware.
 *
 * @param pin Il pin hardware a cui il LED è collegato.
 */
LED::LED(int pin_red, int pin_green, int pin_blue) : Actuator(pin)
{
    this->pin = 0;

    Serial.print("RGB Light setup starting.\n");
    this->pin_red = pin_red;
    this->pin_green = pin_green;
    this->pin_blue = pin_blue;
    this->blink_on = 0;
    this->blink_off = 0;
    state = LightState::OFF;
    color = Color::NONE;
    pinMode(pin_red, OUTPUT);
    pinMode(pin_green, OUTPUT);
    pinMode(pin_blue, OUTPUT);
    digitalWrite(pin_red, LOW);
    digitalWrite(pin_green, LOW);
    digitalWrite(pin_blue, LOW);

    Serial.print("RGB Light setup complete.\n");
}

/**
 * @brief Imposta lo stato del LED (ON o OFF).
 *
 * Cambia lo stato del LED a ON o OFF. Se viene richiesto il lampeggio,
 * la funzione non esegue alcuna modifica.
 *
 * @param state Stato desiderato del LED (ON, OFF).
 */
void LED::set_state(LightState state)
{
    if (state == LightState::BLINK || state == LED::state)
        return;

    LED::state = state;
}

/**
 * @brief Imposta lo stato del LED RGB (ON o OFF).
 *
 * Cambia lo stato del LED RGB a ON o OFF. Se viene richiesto il lampeggio,
 * la funzione non esegue alcuna modifica.
 *
 * @param state Stato desiderato del LED (ON, OFF).
 */
void LED::set_state(LightState state, Color color)
{
    if (state == LightState::BLINK || state == LED::state)
        return;

    LED::state = state;
    LED::color = color;
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
void LED::set_state(int blink_on, int blink_off)
{
    LED::state = LightState::BLINK;
    LED::blink_on = blink_on;
    LED::blink_off = blink_off;
}

/**
 * @brief Imposta il lampeggio del LED RGB.
 *
 * Configura i tempi di accensione e spegnimento per il lampeggio
 * del LED RGB e imposta lo stato a BLINK.
 *
 * @param blink_on Durata del periodo ON (in millisecondi).
 * @param blink_off Durata del periodo OFF (in millisecondi).
 * @param color
 */
void LED::set_state(int blink_on, int blink_off, Color color)
{
    LED::state = LightState::BLINK;
    LED::blink_on = blink_on;
    LED::blink_off = blink_off;
    LED::color = color;
}

void LED::write(bool on, Color color)
{

    if (!on)
    {
        if (color == Color::NONE)
            analogWrite(LED::pin, LOW);
        else
        {
            analogWrite(LED::pin_red, LOW);
            analogWrite(LED::pin_green, LOW);
            analogWrite(LED::pin_blue, LOW);
        }
        return;
    }
    switch (color)
    {
    case Color::RED:
        analogWrite(LED::pin_red, 255);
        analogWrite(LED::pin_green, 0);
        analogWrite(LED::pin_blue, 0);
        break;
    case Color::GREEN:
        analogWrite(LED::pin_red, 0);
        analogWrite(LED::pin_green, 255);
        analogWrite(LED::pin_blue, 0);
        break;
    case Color::BLUE:
        analogWrite(LED::pin_red, 0);
        analogWrite(LED::pin_green, 0);
        analogWrite(LED::pin_blue, 255);
        break;
    case Color::LIGHT_BLUE:
        analogWrite(LED::pin_red, 170);
        analogWrite(LED::pin_green, 170);
        analogWrite(LED::pin_blue, 255);
        break;

    case Color::PURPLE:
        analogWrite(LED::pin_red, 170);
        analogWrite(LED::pin_green, 0);
        analogWrite(LED::pin_blue, 255);
        break;
    case Color::WHITE:
        analogWrite(LED::pin_red, 255);
        analogWrite(LED::pin_green, 255);
        analogWrite(LED::pin_blue, 255);
        break;
    case Color::NONE:
        analogWrite(LED::pin, HIGH);
    default:
        break;
    }
}

/**
 * @brief Aggiorna lo stato del LED.
 *
 * Controlla e aggiorna il comportamento del LED in base al suo stato attuale.
 * Per il lampeggio, utilizza un approccio non bloccante basato su millis().
 * Se il LED non è in modalità BLINK, viene acceso o spento in base allo stato impostato.
 */
void LED::update()
{
    static unsigned long lastUpdate = 0; ///< Memorizza l'ultimo momento in cui il LED è stato aggiornato.
    static bool isOn = false;            ///< Stato corrente del LED durante il lampeggio.

    if (state != LightState::BLINK)
    {
        // Accensione o spegnimento diretto
        write(state == LightState::ON ? true : false, color);
        return;
    }

    // Lampeggio non bloccante
    unsigned long currentMillis = millis();
    unsigned long millisPassed = currentMillis - lastUpdate;

    if (isOn && millisPassed >= (unsigned long)blink_on)
    {
        // Se il LED è acceso e il tempo "on" è trascorso
        write(false, color);
        isOn = false;
        lastUpdate = currentMillis;
    }
    else if (!isOn && millisPassed >= (unsigned long)blink_off)
    {
        // Se il LED è spento e il tempo "off" è trascorso
        write(true, color);
        isOn = true;
        lastUpdate = currentMillis;
    }
}
