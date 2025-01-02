/**
 * @file Receiver.h
 * @brief Dichiarazioni per la classe Receiver.
 * 
 * La classe Receiver rappresenta un'interfaccia per leggere i dati da un ricevitore RC
 * con più canali.
 */

#ifndef RECEIVER_H
#define RECEIVER_H

#include "DataStructures.h"
#include "PWM.hpp"

/**
 * @class Receiver
 * @brief Implementazione di un ricevitore RC multi-canale.
 * 
 * Questa classe consente di leggere i dati provenienti da un ricevitore RC (Radio Control),
 * supportando più canali configurabili.
 */
class Receiver {
private:
    int pins[IA6B_CHANNELS]; ///< Array dei pin hardware associati ai canali del ricevitore.
    int data[IA6B_CHANNELS]; ///< Array dei dati letti dai canali del ricevitore.

public:
    /**
     * @brief Costruttore della classe Receiver.
     * 
     * Inizializza un ricevitore RC associato a un array di pin hardware.
     * 
     * @param pins Puntatore a un array di pin che rappresentano i canali del ricevitore.
     */
    Receiver(int *pins);

    /**
     * @brief Legge i dati dai canali del ricevitore.
     * 
     * Restituisce un array di valori letti dai canali RC.
     * 
     * @return Puntatore all'array contenente i dati dei canali.
     */
    int* read();
};

#endif // RECEIVER_H
