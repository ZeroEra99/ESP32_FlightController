/**
 * @file Actuator.h
 * @brief Dichiarazioni per la classe base Actuator.
 *
 * La classe Actuator rappresenta un'interfaccia astratta per componenti fisici
 * controllabili come esc, servomotori o LED.
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
 * garantendo che ogni derivato implementi il metodo `write`.
 * Gli attuatori sono dispositivi hardware controllati tramite segnali inviati
 * ai pin specifici dell'ESP32.
 */
class Actuator
{
protected:
  int pin; ///< Pin hardware associato all'attuatore.

public:
  /**
   * @brief Costruttore della classe Actuator.
   *
   * Inizializza l'attuatore associandolo al pin specificato.
   *
   * @param pin Il pin hardware a cui l'attuatore è collegato.
   */
  Actuator(int pin) : pin(pin) {};

  /**
   * @brief Scrive un valore sull'attuatore.
   *
   * Metodo astratto che deve essere implementato dalle classi derivate.
   * Ogni implementazione deve specificare come inviare un valore
   * (es. angolo per un servo o intensità per un LED).
   *
   * @param value Il valore da inviare all'attuatore.
   */
  virtual void write(int value) = 0;
};

#endif // ACTUATOR_H
