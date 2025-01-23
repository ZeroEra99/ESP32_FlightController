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
public:
    /**
     * @brief Costruttore della classe Actuator.
     *
     * @param pin Pin associato all'attuatore.
     */
    explicit Actuator(int pin);

    /**
     * @brief Imposta il valore del segnale PWM per l'attuatore.
     *
     * @param value Valore del segnale PWM (0-1).
     */
    virtual void write(double value) = 0;
};

class ESC : public Actuator
{
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
     * @param value Valore del segnale PWM.
     */
    void write(double value) override;
};

class ServoMotor : public Actuator
{
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
     * @param value Valore del segnale PWM.
     */
    void write(double value) override;
};



#endif // ACTUATOR_H
