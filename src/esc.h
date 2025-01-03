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
 * È progettata per gestire motori brushless con precisione e sicurezza, rispettando i limiti configurati
 * per il segnale PWM.
 */
class ESC : public Actuator
{
private:
    Servo esc;   ///< Oggetto Servo per la gestione del segnale PWM.
    int pwm_min; ///< Valore minimo del segnale PWM (in microsecondi).
    int pwm_max; ///< Valore massimo del segnale PWM (in microsecondi).

public:
    /**
     * @brief Costruttore della classe ESC.
     *
     * Inizializza un ESC associato a un determinato pin hardware e definisce i valori
     * minimo e massimo del segnale PWM.
     *
     * @param pin Il pin hardware a cui l'ESC è collegato.
     * @param pwm_min Valore minimo del segnale PWM (in microsecondi).
     * @param pwm_max Valore massimo del segnale PWM (in microsecondi).
     */
    ESC(int pin, int pwm_min, int pwm_max);

    /**
     * @brief Scrive un valore sull'ESC.
     *
     * Invio del segnale PWM al motore controllato dall'ESC, mappato nel range definito.
     * Se il valore fornito è al di fuori del range configurato, verrà automaticamente
     * limitato ai valori `pwm_min` o `pwm_max`.
     *
     * @param value Il valore da inviare all'ESC (in microsecondi).
     */
    void write(int value) override;
};

#endif // ESC_H
