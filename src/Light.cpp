#include "Light.h"

Light::Light(int pin, Color color) : Actuator(pin)
{
    Serial.print("Light ");
    Serial.print(pin);
    color = color;
    state = LightState::OFF;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    Serial.print(" setup complete.\n");
}

void Light::set_state(LightState state)
{
    if (state == LightState::BLINK)
        return;
    if (Light::state != state)
        Light::state = state;
    else
        return;
}

void Light::set_state(int blink_on, int blink_off)
{
    Light::state = LightState::BLINK;
    Light::blink_on = blink_on;
    Light::blink_off = blink_off;
}

void Light::update()
{
    static unsigned long lastUpdate = 0; // Memorizza l'ultimo momento in cui il LED è stato aggiornato
    static bool isOn = false;            // Stato corrente del LED durante il lampeggio

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