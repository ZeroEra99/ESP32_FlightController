/**
 * @file ServoMotor.h
 * @brief Dichiarazione della classe ServoMotor per il controllo dei motori.
 */

#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include "Actuator.h"

/**
 * @brief Classe per la gestione dei motori.
 * 
 * Estende la classe Actuator per includere il controllo specifico dei motori.
 */
class ServoMotor : public Actuator
{
private:
    Servo servo; ///< Oggetto Servo per controllare il motore.

public:
    /**
     * @brief Costruttore della classe ServoMotor.
     * 
     * Inizializza il servomotore sul pin specificato.
     * 
     * @param pin Pin associato al servomotore.
     */
    ServoMotor(int pin);

    /**
     * @brief Imposta il valore di controllo del servomotore.
     * 
     * Converte il valore digitale in PWM e lo invia al motore.
     * 
     * @param value Valore digitale da impostare.
     */
    void write(double value);
};

#endif // SERVO_MOTOR_H
