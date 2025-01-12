#include "LED.h"
//#include "DebugLogger.h"

LED::LED(int pin) : Actuator(pin)
{
    Serial.println("Light setup starting.");
/*
    this->pin_red = 0;
    this->pin_green = 0;
    this->pin_blue = 0;

/*
    DebugLogger::getInstance()->log("Light ", LogLevel::DEBUG);
    DebugLogger::getInstance()->log(pin, LogLevel::DEBUG);
    DebugLogger::getInstance()->log(" setup starting.", LogLevel::DEBUG);

    
    this->pin = pin;
    this->blink_on = 0;
    this->blink_off = 0;
    state = LED_STATE::OFF;
    color = {};
    pinMode(pin, OUTPUT);
    ledcWrite(pin, LOW);
*/
    Serial.println("Light setup complete.");

    //DebugLogger::getInstance()->log("Light setup complete.", LogLevel::DEBUG);
}

LED::LED(int pin_red, int pin_green, int pin_blue) : Actuator(pin)
{
    // Configurazione di un LED RGB
    this->pin = 0;

    //DebugLogger::getInstance()->log("RGB Light setup starting.", LogLevel::DEBUG);
    this->pin_red = pin_red;
    this->pin_green = pin_green;
    this->pin_blue = pin_blue;
    this->blink_on = 0;
    this->blink_off = 0;
    state = LED_STATE::OFF;
    color = COLOR::NONE;
    pinMode(pin_red, OUTPUT);
    pinMode(pin_green, OUTPUT);
    pinMode(pin_blue, OUTPUT);
    ledcWrite(pin_red, LOW);
    ledcWrite(pin_green, LOW);
    ledcWrite(pin_blue, LOW);

    //DebugLogger::getInstance()->log("RGB Light setup complete.", LogLevel::DEBUG);
}

void LED::set_state(LED_STATE state)
{
    // Imposta lo stato del LED (acceso/spento)
    if (state == LED_STATE::BLINK || state == LED::state)
        return;

    LED::state = state;
}

void LED::set_state(LED_STATE state, COLOR color)
{
    // Imposta lo stato e il colore del LED
    if (state == LED_STATE::BLINK || state == LED::state)
        return;

    LED::state = state;
    LED::color = color;
}

void LED::set_state(int blink_on, int blink_off)
{
    // Imposta il LED in modalità lampeggio
    LED::state = LED_STATE::BLINK;
    LED::blink_on = blink_on;
    LED::blink_off = blink_off;
}

void LED::set_state(int blink_on, int blink_off, COLOR color)
{
    // Imposta il lampeggio e il colore del LED
    LED::state = LED_STATE::BLINK;
    LED::blink_on = blink_on;
    LED::blink_off = blink_off;
    LED::color = color;
}

void LED::write(bool on, COLOR color)
{
    // Scrive lo stato e il colore del LED
    if (!on)
    {
        if (color == COLOR::NONE)
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
    case COLOR::RED:
        analogWrite(LED::pin_red, 255);
        analogWrite(LED::pin_green, 0);
        analogWrite(LED::pin_blue, 0);
        break;
    case COLOR::GREEN:
        analogWrite(LED::pin_red, 0);
        analogWrite(LED::pin_green, 255);
        analogWrite(LED::pin_blue, 0);
        break;
    case COLOR::BLUE:
        analogWrite(LED::pin_red, 0);
        analogWrite(LED::pin_green, 0);
        analogWrite(LED::pin_blue, 255);
        break;
    case COLOR::LIGHT_BLUE:
        analogWrite(LED::pin_red, 170);
        analogWrite(LED::pin_green, 170);
        analogWrite(LED::pin_blue, 255);
        break;

    case COLOR::PURPLE:
        analogWrite(LED::pin_red, 170);
        analogWrite(LED::pin_green, 0);
        analogWrite(LED::pin_blue, 255);
        break;
    case COLOR::WHITE:
        analogWrite(LED::pin_red, 255);
        analogWrite(LED::pin_green, 255);
        analogWrite(LED::pin_blue, 255);
        break;
    case COLOR::NONE:
        analogWrite(LED::pin, HIGH);
    default:
        break;
    }
}

void LED::update()
{
    static unsigned long lastUpdate = 0; ///< Memorizza l'ultimo momento in cui il LED è stato aggiornato.
    static bool isOn = false;            ///< Stato corrente del LED durante il lampeggio.

    if (state != LED_STATE::BLINK)
    {
        // Accensione o spegnimento diretto
        write(state == LED_STATE::ON ? true : false, color);
        return;
    }

    // Lampeggio non bloccante
    unsigned long currentMillis = millis();
    unsigned long millisPassed = currentMillis - lastUpdate;

    if (isOn && millisPassed >= (unsigned long)blink_on)
    {
        // Spegne il LED dopo il tempo ON
        write(false, color);
        isOn = false;
        lastUpdate = currentMillis;
    }
    else if (!isOn && millisPassed >= (unsigned long)blink_off)
    {
        // Accende il LED dopo il tempo OFF
        write(true, color);
        isOn = true;
        lastUpdate = currentMillis;
    }
}
