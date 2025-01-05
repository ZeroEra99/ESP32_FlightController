/**
 * @file esc.h
 * @brief Dichiarazione della classe ESC per il controllo degli Electronic Speed Controller.
 */

#ifndef ESC_H
#define ESC_H

#include "Actuator.h"

/**
 * @brief Classe per la gestione degli ESC (Electronic Speed Controller).
 * 
 * Consente il controllo del segnale PWM per pilotare i motori attraverso un ESC.
 */
class ESC : public Actuator
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
    ESC(int pin);

    /**
     * @brief Imposta il valore di controllo dell'ESC.
     * 
     * Converte il valore digitale in PWM e lo invia all'ESC.
     * 
     * @param value Valore digitale da impostare.
     */
    void write(double value);
};

#endif // ESC_H
