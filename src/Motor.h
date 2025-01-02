/**
 * @file Motor.h
 * @brief Dichiarazioni per la classe Motor.
 * 
 * La classe Motor rappresenta un attuatore basato su un servomotore,
 * controllato tramite segnali PWM.
 */

#ifndef MOTOR_H
#define MOTOR_H

#include "Actuator.h"

/**
 * @class Motor
 * @brief Implementazione di un attuatore per un servomotore.
 * 
 * Questa classe estende Actuator e fornisce un'interfaccia per controllare
 * un servomotore con angoli minimi e massimi configurabili.
 */
class Motor : public Actuator
{
private:
    Servo servo; ///< Oggetto Servo per la gestione del segnale PWM.
    int angle_min; ///< Angolo minimo del servomotore.
    int angle_max; ///< Angolo massimo del servomotore.

public:
    /**
     * @brief Costruttore della classe Motor.
     * 
     * Inizializza un servomotore associato a un determinato pin hardware e definisce
     * i limiti minimo e massimo per l'angolo di movimento.
     * 
     * @param pin Il pin hardware a cui il servomotore è collegato.
     * @param angle_min Angolo minimo del servomotore (in gradi).
     * @param angle_max Angolo massimo del servomotore (in gradi).
     */
    Motor(int pin, int angle_min, int angle_max);

    /**
     * @brief Scrive un valore sul servomotore.
     * 
     * Imposta l'angolo del servomotore in base al valore specificato,
     * rispettando i limiti definiti durante l'inizializzazione.
     * 
     * @param value Il valore da inviare al servomotore (in gradi).
     */
    void write(int value) override;
};

#endif // MOTOR_H
