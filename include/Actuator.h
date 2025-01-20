/**
 * @file Actuator.h
 * @brief Dichiarazione delle classi per la gestione degli attuatori.
 */

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <ESP32Servo.h>

class ESC 
{
private:
    int pin;           ///< Pin associato all'ESC.
    int pwmChannel;    ///< Canale PWM utilizzato dall'ESP32.
    int frequency;     ///< Frequenza del segnale PWM (tipicamente 50 Hz per ESC).
    int resolution;    ///< Risoluzione del segnale PWM (es. 16 bit).

public:
    /**
     * @brief Costruttore della classe ESC.
     * 
     * Inizializza l'ESC sul pin specificato.
     * 
     * @param pin Pin associato all'ESC.
     * @param pwmChannel Canale PWM da utilizzare (0-15).
     * @param frequency Frequenza PWM (default: 50 Hz).
     * @param resolution Risoluzione PWM in bit (default: 16 bit).
     */
    explicit ESC(int pin, int pwmChannel, int frequency = 50, int resolution = 16);

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
