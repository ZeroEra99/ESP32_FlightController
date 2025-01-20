#ifndef ACTUATOR_H
#define ACTUATOR_H

class ESC
{
private:
    int pin;        ///< Pin associato all'ESC.
    int pwmChannel; ///< Canale PWM utilizzato dall'ESP32.
    int frequency;  ///< Frequenza del segnale PWM (tipicamente 50 Hz per ESC).
    int resolution; ///< Risoluzione del segnale PWM (es. 16 bit).

public:
    explicit ESC(int pin, int pwmChannel, int frequency = 50, int resolution = 11);
    void writeESC(double value);
};

class ServoMotor
{
private:
    int pin;        ///< Pin associato al servomotore.
    int pwmChannel; ///< Canale PWM utilizzato dall'ESP32.
    int frequency;  ///< Frequenza del segnale PWM (50 Hz per servomotori).
    int resolution; ///< Risoluzione PWM in bit (ad esempio, 16 bit).

public:
    explicit ServoMotor(int pin, int pwmChannel, int frequency = 50, int resolution = 11);
    void writeServo(double value);
};

#endif // ACTUATOR_H
