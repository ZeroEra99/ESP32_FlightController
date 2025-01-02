/**
 * @file Motor.cpp
 * @brief Implementazione della classe Motor.
 *
 * Questo file contiene l'implementazione delle funzioni della classe Motor,
 * utilizzata per controllare un servomotore.
 */

#include "Motor.h"

/**
 * @brief Costruttore della classe Motor.
 *
 * Inizializza un servomotore associato a un determinato pin hardware e configura
 * i limiti minimo e massimo per l'angolo di movimento.
 *
 * @param pin Il pin hardware a cui il servomotore è collegato.
 * @param angle_min Angolo minimo del servomotore (in gradi).
 * @param angle_max Angolo massimo del servomotore (in gradi).
 */
Motor::Motor(int pin, int angle_min, int angle_max) : Actuator(pin)
{
    Serial.print("Servo ");
    Serial.print(pin);
    Serial.print(" setup starting.\n");

    this->angle_min = angle_min;
    this->angle_max = angle_max;
    servo.attach(pin);

    Serial.print("Servo ");
    Serial.print(pin);
    Serial.print(" setup complete.\n");
}

/**
 * @brief Scrive un valore sul servomotore.
 *
 * Imposta l'angolo del servomotore in base al valore specificato. Il valore
 * deve essere compreso tra `angle_min` e `angle_max`.
 *
 * @param value Il valore da inviare al servomotore (in gradi).
 */
void Motor::write(int value)
{
    // Scrittura del valore sul servomotore
    servo.write(value);
}
