/**
 * @file Actuator.h
 *
 * @brief Dichiarazione delle classi per la gestione degli attuatori.
 */
#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <ESP32Servo.h>

class Actuator
{
protected:
    Servo actuator; ///< Oggetto Servo associato all'attuatore.
    int pin;        ///< Pin associato all'attuatore.
    int pwm_min, pwm_max, pwm_null;
    double digital_min, digital_max;

public:
    /**
     * @brief Costruttore della classe Actuator.
     *
     * @param pin Pin associato all'attuatore.
     */
    explicit Actuator(int pin, int pwm_min, int pwm_max, int pwm_null, double digital_min, double digital_max);

    /**
     * @brief Imposta il valore del segnale PWM per l'attuatore.
     *
     * @param value Valore del segnale PWM (0-1).
     */
    void write(double value);
};


#endif // ACTUATOR_H
