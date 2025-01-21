/**
 * @file Actuator.h
 *
 * @brief Dichiarazione delle classi per la gestione degli attuatori.
 */
#ifndef ACTUATOR_H
#define ACTUATOR_H

/**
 * @brief Classe per la gestione di un ESC (Electronic Speed Controller).
 *
 * Questa classe permette di controllare un ESC tramite un segnale PWM.
 * L'ESC è un dispositivo elettronico che controlla la velocità di un motore brushless.
 */
class ESC
{
private:
    int pin;        ///< Pin associato all'ESC.
    int pwmChannel; ///< Canale PWM utilizzato dall'ESP32.
    int frequency;  ///< Frequenza del segnale PWM (tipicamente 50 Hz per ESC).
    int resolution; ///< Risoluzione del segnale PWM (es. 16 bit).

public:
    /**
     * @brief Costruttore della classe ESC.
     *
     * @param pin Pin associato all'ESC.
     * @param pwmChannel Canale PWM utilizzato dall'ESP32.
     * @param frequency Frequenza del segnale PWM (default: 50 Hz).
     * @param resolution Risoluzione del segnale PWM (default: 11 bit).
     */
    explicit ESC(int pin, int pwmChannel, int frequency = 50, int resolution = 11);

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
    int pin;        ///< Pin associato al servomotore.
    int pwmChannel; ///< Canale PWM utilizzato dall'ESP32.
    int frequency;  ///< Frequenza del segnale PWM (50 Hz per servomotori).
    int resolution; ///< Risoluzione PWM in bit (ad esempio, 16 bit).

public:
    /**
     * @brief Costruttore della classe ServoMotor.
     *
     * @param pin Pin associato al servomotore.
     * @param pwmChannel Canale PWM utilizzato dall'ESP32.
     * @param frequency Frequenza del segnale PWM (default: 50 Hz).
     * @param resolution Risoluzione del segnale PWM (default: 11 bit).
     */
    explicit ServoMotor(int pin, int pwmChannel, int frequency = 50, int resolution = 11);

    /**
     * @brief Imposta il valore del segnale PWM per il servomotore.
     *
     * @param value Valore del segnale PWM (0-1).
     */
    void writeServo(double value);
};

#endif // ACTUATOR_H
