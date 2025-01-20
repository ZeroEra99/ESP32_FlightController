#include "LED.h"
#include "Logger.h"
#include <Arduino.h>

LED::LED(int pin)
{
    // Configurazione di un LED
    this->pin = pin;
    this->blink_on = 0;
    this->blink_off = 0;
    state = LED_STATE::OFF;
    // Inizializzazione del LED
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delay(500);
    digitalWrite(pin, LOW);
    Logger::getInstance().log(LogLevel::INFO, "LED setup complete.");
}

void LED::set_state(LED_STATE state)
{
    // Se lo stato del LED è diverso da quello attuale, lo aggiorniamo
    if (this->state != state)
        this->state = state;
}

void LED::set_state(int blink_on, int blink_off)
{
    // Imposta il LED in modalità lampeggio
    if (this->state != LED_STATE::BLINK)
        this->state = LED_STATE::BLINK;
    // Aggiorna i tempi di lampeggio
    if (this->blink_on != blink_on)
        this->blink_on = blink_on;
    if (this->blink_off != blink_off)
        this->blink_off = blink_off;
}

void LED::update()
{
    unsigned long currentMillis = millis(); // Tempo corrente

    if (state == LED_STATE::OFF)
    {
        // Se il LED è spento, lo spegniamo
        digitalWrite(pin, LOW);
    }
    else if (state == LED_STATE::ON)
    {
        // Se il LED è acceso, lo accendiamo
        digitalWrite(pin, HIGH);
    }
    else if (state == LED_STATE::BLINK)
    {
        // Se il LED è in modalità lampeggio
        static unsigned long previousMillis_simple = 0; // Variabile statica unica per il LED semplice
        static bool ledState_simple = false;

        // Alterna lo stato del LED in base al tempo
        if (currentMillis - previousMillis_simple >= (ledState_simple ? blink_on : blink_off))
        {
            previousMillis_simple = currentMillis; // Aggiorna il tempo
            ledState_simple = !ledState_simple;    // Cambia lo stato del LED

            // Accende o spegne il LED a seconda dello stato
            digitalWrite(pin, ledState_simple ? HIGH : LOW);
        }
    }
}

RGB_LED::RGB_LED(int pin_red, int channel_red, int pin_green, int channel_green, int pin_blue, int channel_blue, int freq, int resolution)
    : pin_red(pin_red), channel_red(channel_red),
      pin_green(pin_green), channel_green(channel_green),
      pin_blue(pin_blue), channel_blue(channel_blue),
      state(LED_STATE::OFF), color(COLOR::NONE),
      blink_on(0), blink_off(0)
{
    // Configura il PWM per ciascun canale
    ledcSetup(channel_red, freq, resolution);
    ledcSetup(channel_green, freq, resolution);
    ledcSetup(channel_blue, freq, resolution);

    // Associa ciascun pin al relativo canale PWM
    ledcAttachPin(pin_red, channel_red);
    ledcAttachPin(pin_green, channel_green);
    ledcAttachPin(pin_blue, channel_blue);

    // Accendi tutti i colori
    ledcWrite(channel_red, 255);
    ledcWrite(channel_green, 255);
    ledcWrite(channel_blue, 255);

    delay(500);

    // Spegni tutti i colori
    ledcWrite(channel_red, 0);
    ledcWrite(channel_green, 0);
    ledcWrite(channel_blue, 0);
    Logger::getInstance().log(LogLevel::INFO, "RGB Light setup complete.");
}

void RGB_LED::set_state(LED_STATE state)
{
    // Se lo stato del LED RGB è diverso da quello attuale, lo aggiorniamo
    if (state == LED_STATE::OFF && this->state != LED_STATE::OFF)
        this->state = state;
}

void RGB_LED::set_state(LED_STATE state, COLOR color)
{
    // Imposta lo stato e il colore del LED RGB
    if (this->state != state && state != LED_STATE::BLINK)
        this->state = state;
    if (this->color != color)
        this->color = color;
}

void RGB_LED::set_state(int blink_on, int blink_off, COLOR color)
{
    // Imposta il LED RGB in modalità lampeggio
    if (this->state != LED_STATE::BLINK)
        this->state = LED_STATE::BLINK;
    // Aggiorna i tempi di lampeggio e il colore
    if (this->blink_on != blink_on)
        this->blink_on = blink_on;
    if (this->blink_off != blink_off)
        this->blink_off = blink_off;
    if (this->color != color)
        this->color = color;
}

void RGB_LED::write(bool on, COLOR color)
{
    // Scrive lo stato e il colore del LED
    int color_values[3] = {0, 0, 0}; // R, G, B

    // Imposta il colore del LED RGB in base al colore selezionato
    switch (color)
    {
    case COLOR::RED:
        color_values[0] = 255;
        break;
    case COLOR::GREEN:
        color_values[1] = 255;
        break;
    case COLOR::BLUE:
        color_values[2] = 255;
        break;
    case COLOR::LIGHT_BLUE:
        color_values[0] = 170;
        color_values[1] = 170;
        color_values[2] = 255;
        break;
    case COLOR::PURPLE:
        color_values[0] = 170;
        color_values[2] = 255;
        break;
    case COLOR::WHITE:
        color_values[0] = 255;
        color_values[1] = 255;
        color_values[2] = 255;
        break;
    default:
        break;
    }

    // Se lo stato è OFF, spegni tutti i canali
    if (!on)
    {
        ledcWrite(channel_red, 0);
        ledcWrite(channel_green, 0);
        ledcWrite(channel_blue, 0);
        return;
    }

    // Scrive i valori dei colori sui rispettivi canali PWM
    ledcWrite(channel_red, color_values[0]);
    ledcWrite(channel_green, color_values[1]);
    ledcWrite(channel_blue, color_values[2]);
}

void RGB_LED::update()
{
    unsigned long currentMillis = millis(); // Tempo corrente

    if (state == LED_STATE::OFF)
    {
        // Se il LED RGB è spento, spegniamo tutti i colori
        ledcWrite(0, 0);
        ledcWrite(1, 0);
        ledcWrite(2, 0);
    }
    else if (state == LED_STATE::ON)
    {
        // Se il LED RGB è acceso, scriviamo il colore attuale
        write(true, color);
    }
    else if (state == LED_STATE::BLINK)
    {
        // Se il LED RGB è in modalità lampeggio
        static unsigned long previousMillis_rgb = 0; // Variabile statica unica per il LED RGB
        static bool ledState_rgb = false;

        // Alterna lo stato del LED in base al tempo
        if (currentMillis - previousMillis_rgb >= (ledState_rgb ? blink_on : blink_off))
        {
            previousMillis_rgb = currentMillis; // Aggiorna il tempo
            ledState_rgb = !ledState_rgb;       // Cambia lo stato del LED

            // Accende o spegne il LED a seconda dello stato
            write(ledState_rgb, color);
        }
    }
}
