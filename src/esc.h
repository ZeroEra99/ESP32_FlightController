/**
 * @file ESC.h
 * @brief Dichiarazioni per la classe ESC.
 *
 * La classe ESC rappresenta un controller elettronico di velocità (Electronic Speed Controller),
 * utilizzato per controllare motori brushless tramite segnali PWM.
 */

#ifndef ESC_H
#define ESC_H

#include "Actuator.h"

/**
 * @class ESC
 * @brief Implementazione di un attuatore per un controller elettronico di velocità (ESC).
 *
 * Questa classe estende Actuator e fornisce un'interfaccia per controllare un ESC utilizzando segnali PWM.
 */
class ESC : public Actuator
{
private:
    Servo esc;   ///< Oggetto Servo per la gestione del segnale PWM.
    int pwm_min; ///< Valore minimo del segnale PWM.
    int pwm_max; ///< Valore massimo del segnale PWM.

public:
    /**
     * @brief Costruttore della classe ESC.
     *
     * Inizializza un ESC associato a un determinato pin hardware e definisce i valori
     * minimo e massimo del segnale PWM.
     *
     * @param pin Il pin hardware a cui l'ESC è collegato.
     * @param analog_min Valore minimo del segnale PWM (analogico).
     * @param analog_max Valore massimo del segnale PWM (analogico).
     */
    ESC(int pin, int analog_min, int analog_max);

    /**
     * @brief Scrive un valore sull'ESC.
     *
     * Invio del segnale PWM al motore controllato dall'ESC, mappato nel range definito.
     *
     * @param value Il valore da inviare all'ESC.
     */
    void write(int value) override;
};

#endif // ESC_H
