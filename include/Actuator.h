/**
 * @file Actuator.h
 *
 * @brief Dichiarazione delle classi per la gestione degli attuatori.
 */
#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <ESP32Servo.h>

/**
 * @brief Classe per la gestione di un ESC (Electronic Speed Controller).
 *
 * Questa classe permette di controllare un ESC tramite un segnale PWM.
 * L'ESC è un dispositivo elettronico che controlla la velocità di un motore brushless.
 */
class ESC
{
private:
    Servo esc; ///< Oggetto Servo associato all'ESC.
    int pin;   ///< Pin associato all'ESC.

public:
    /**
     * @brief Costruttore della classe ESC.
     *
     * @param pin Pin associato all'ESC.
     */
    explicit ESC(int pin);

    /**
     * @brief Imposta il valore del segnale PWM per l'ESC.
     *
     * @param value Valore del segnale PWM (0-1).
     */
    void writeESC(double value);
};

/**
 * @brief Classe per la gestione di un servomotore.
 *
 * Questa classe permette di controllare un servomotore tramite un segnale PWM.
 * Il servomotore è un dispositivo che permette di controllare una superficie di controllo.
 */
class ServoMotor
{
private:
    Servo servo; ///< Oggetto Servo associato al servomotore.
    int pin;     ///< Pin associato al servomotore.

public:
    /**
     * @brief Costruttore della classe ServoMotor.
     *
     * @param pin Pin associato al servomotore.
     */
    explicit ServoMotor(int pin);

    /**
     * @brief Imposta il valore del segnale PWM per il servomotore.
     *
     * @param value Valore del segnale PWM (0-1).
     */
    void writeServo(double value);
};

#endif // ACTUATOR_H
