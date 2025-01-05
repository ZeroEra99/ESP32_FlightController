/**
 * @file Actuator.h
 * @brief Dichiarazione della classe base Actuator per la gestione degli attuatori.
 */

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "DataStructures.h"
#include <ESP32Servo.h>

/**
 * @brief Classe base per la gestione degli attuatori.
 *
 * Fornisce funzionalità comuni per la conversione tra valori digitali e PWM.
 */
class Actuator
{
protected:
  /**
   * @brief Converte un valore digitale in PWM.
   *
   * @param value Valore digitale da convertire.
   * @param min_digital Valore digitale minimo.
   * @param max_digital Valore digitale massimo.
   * @param min_analog Valore PWM minimo.
   * @param max_analog Valore PWM massimo.
   * @return Valore PWM corrispondente.
   */
  int digital_to_pwm(double value, double min_digital, double max_digital, int min_analog, int max_analog);

  /**
   * @brief Converte un valore PWM in digitale.
   *
   * @param pwm_value Valore PWM da convertire.
   * @param min_analog Valore PWM minimo.
   * @param max_analog Valore PWM massimo.
   * @param min_digital Valore digitale minimo.
   * @param max_digital Valore digitale massimo.
   * @return Valore digitale corrispondente.
   */
  double pwm_to_digital(int pwm_value, int min_analog, int max_analog, double min_digital, double max_digital);

  int pin; ///< Pin dell'attuatore.

public:
  /**
   * @brief Costruttore per la classe Actuator.
   *
   * @param pin Pin associato all'attuatore.
   */
  Actuator(int pin) : pin(pin) {};
};

#endif // ACTUATOR_H
