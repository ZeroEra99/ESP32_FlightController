/**
 * @file Receiver.h
 * @brief Dichiarazioni per la classe Receiver.
 *
 * La classe Receiver rappresenta un'interfaccia per leggere i dati da un ricevitore RC
 * con più canali, mappando i segnali PWM ricevuti in valori digitali utilizzabili.
 */

#ifndef RECEIVER_H
#define RECEIVER_H

#include "DataStructures.h"
#include "PWM.hpp"

/**
 * @struct PWMs
 * @brief Struttura che contiene gli oggetti PWM del ricevitore.
 *
 * Ogni membro della struttura rappresenta un canale PWM specifico del ricevitore RC.
 */
struct PWMs {
    PWM pwm_roll;     ///< PWM per il controllo del rollio.
    PWM pwm_pitch;    ///< PWM per il controllo del beccheggio.
    PWM pwm_throttle; ///< PWM per il controllo del throttle.
    PWM pwm_yaw;      ///< PWM per il controllo dell'imbardata.
    PWM pwm_swa;      ///< PWM per lo switch A.
    PWM pwm_swb;      ///< PWM per lo switch B.
    PWM pwm_swc;      ///< PWM per lo switch C.
    PWM pwm_swd;      ///< PWM per lo switch D.
    PWM pwm_vra;      ///< PWM per il potenziometro A.
    PWM pwm_vrb;      ///< PWM per il potenziometro B.
};

/**
 * @class Receiver
 * @brief Implementazione di un ricevitore RC multi-canale.
 *
 * Questa classe consente di leggere i dati provenienti da un ricevitore RC (Radio Control),
 * supportando più canali configurabili. I dati letti vengono mappati in una struttura `PilotData`
 * per una facile integrazione con altre componenti del sistema.
 */
class Receiver
{
private:
    ReceiverPins receiver_pins; ///< Pin hardware associati ai canali del ricevitore.
    PilotData data;             ///< Dati letti dai canali del ricevitore.

public:
    /**
     * @brief Costruttore della classe Receiver.
     *
     * Inizializza un ricevitore RC associato a un array di pin hardware. Ogni pin
     * rappresenta un canale del ricevitore.
     *
     * @param receiver_pins Struttura contenente i pin associati ai canali del ricevitore.
     */
    Receiver(ReceiverPins receiver_pins);

    /**
     * @brief Legge i dati dai canali del ricevitore.
     *
     * Legge i segnali PWM ricevuti dai canali e li converte in una struttura `PilotData`.
     *
     * @return Struttura `PilotData` contenente i dati letti dai canali del ricevitore.
     */
    PilotData read();
};

#endif // RECEIVER_H
