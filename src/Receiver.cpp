/**
 * @file Receiver.cpp
 * @brief Implementazione della classe Receiver.
 * 
 * Questo file contiene l'implementazione delle funzioni della classe Receiver,
 * utilizzata per leggere i dati da un ricevitore RC multi-canale IA6B.
 */

#include "Receiver.h"

PWM analog_input[IA6B_CHANNELS]; ///< Oggetti PWM per la lettura dei segnali dai canali del ricevitore.

/**
 * @brief Costruttore della classe Receiver.
 * 
 * Inizializza il ricevitore RC associando ciascun canale a un pin hardware
 * e configurando i corrispondenti oggetti PWM.
 * 
 * @param pins Puntatore a un array di pin che rappresentano i canali del ricevitore.
 */
Receiver::Receiver(int *pins) {
    for (int i = 0; i < IA6B_CHANNELS; i++) {
        analog_input[i].attach(pins[i]);
        analog_input[i].begin(true); ///< Configura l'input PWM per ciascun canale.
    }
}

/**
 * @brief Legge i dati dai canali del ricevitore.
 * 
 * Recupera i valori PWM da ciascun canale e li memorizza nell'array `data`.
 * 
 * @return Puntatore all'array contenente i dati dei canali RC.
 */
int* Receiver::read() {
    for (int i = 0; i < IA6B_CHANNELS; i++) {
        data[i] = analog_input[i].getValue(); ///< Ottiene il valore PWM dal canale corrente.
    }
    return data;
}
