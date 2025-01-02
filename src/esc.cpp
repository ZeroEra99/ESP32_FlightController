/**
 * @file ESC.cpp
 * @brief Implementazione della classe ESC.
 *
 * Questo file contiene l'implementazione delle funzioni della classe ESC,
 * utilizzata per controllare un controller elettronico di velocità (ESC).
 */

#include "ESC.h"

/**
 * @brief Costruttore della classe ESC.
 *
 * Inizializza un ESC associato a un determinato pin hardware e configura
 * i limiti minimo e massimo per il segnale PWM.
 *
 * @param pin Il pin hardware a cui l'ESC è collegato.
 * @param pwm_min Valore minimo del segnale PWM (in microsecondi).
 * @param pwm_max Valore massimo del segnale PWM (in microsecondi).
 */
ESC::ESC(int pin, int pwm_min, int pwm_max) : Actuator(pin)
{
  Serial.print("ESC ");
  Serial.print(pin);
  Serial.print(" setup starting.\n");

  this->pwm_min = pwm_min; ///< Inizializza il limite minimo PWM
  this->pwm_max = pwm_max; ///< Inizializza il limite massimo PWM
  esc.attach(pin);
  esc.writeMicroseconds(pwm_min); ///< Imposta il segnale PWM al valore minimo iniziale

  Serial.print("ESC ");
  Serial.print(pin);
  Serial.print(" setup complete.\n");
}

/**
 * @brief Scrive un valore sull'ESC.
 *
 * Imposta il segnale PWM inviato all'ESC. Il valore deve essere compreso tra
 * `pwm_min` e `pwm_max` per garantire un funzionamento sicuro.
 *
 * @param value Il valore da inviare all'ESC (in microsecondi).
 */
void ESC::write(int value)
{
  // Scrittura del valore sull'ESC
  esc.writeMicroseconds(value);
}
