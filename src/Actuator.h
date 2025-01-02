/**
 * @file Actuator.h
 * @brief Dichiarazioni per la classe base Actuator.
 *
 * La classe Actuator rappresenta un'interfaccia astratta per componenti fisici
 * controllabili, come motori o LED.
 */

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "DataStructures.h"
#include <ESP32Servo.h>

/**
 * @class Actuator
 * @brief Classe base astratta per rappresentare attuatori.
 *
 * Questa classe fornisce un'interfaccia comune per tutti gli attuatori,
 * garantendo che ogni derivato implementi la funzione `write`.
 */
class Actuator
{
protected:
  int pin; ///< Pin hardware associato all'attuatore.

public:
  /**
   * @brief Costruttore della classe Actuator.
   *
   * @param pin Il pin hardware a cui l'attuatore è collegato.
   */
  Actuator(int pin);

  /**
   * @brief Scrive un valore sull'attuatore.
   *
   * Metodo astratto che deve essere implementato dalle classi derivate
   * per specificare come inviare un valore all'attuatore.
   *
   * @param value Il valore da inviare all'attuatore.
   */
  virtual void write(int value) = 0;
};

#endif // ACTUATOR_H
