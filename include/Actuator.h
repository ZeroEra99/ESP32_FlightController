/**
 * @file Actuator.h
 * @brief Dichiarazione delle classi per la gestione degli attuatori.
 */

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <ESP32Servo.h>

/**
 * @brief Classe per la gestione degli ESC (Electronic Speed Controller).
 * 
 * Consente il controllo del segnale PWM per pilotare i motori attraverso un ESC.
 */
class ESC 
{
private:
    Servo esc; ///< Oggetto Servo per la gestione del segnale PWM.

public:
    /**
     * @brief Costruttore della classe ESC.
     * 
     * Inizializza l'ESC sul pin specificato.
     * 
     * @param pin Pin associato all'ESC.
     */
    explicit ESC(int pin);

    /**
     * @brief Imposta il valore di controllo dell'ESC.
     * 
     * Converte il valore digitale in PWM e lo invia all'ESC.
     * 
     * @param value Valore digitale da impostare.
     */
    void write(double value);
};


/**
 * @brief Classe per la gestione dei servoMotori.
 * 
 * Consente il controllo delle superfici di controllo attraverso servomotori.
 */
class ServoMotor
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
    explicit ServoMotor(int pin);

    /**
     * @brief Imposta il valore di controllo del servomotore.
     * 
     * Converte il valore digitale in PWM e lo invia al motore.
     * 
     * @param value Valore digitale da impostare.
     */
    void write(double value);
};


#endif // ACTUATOR_H
