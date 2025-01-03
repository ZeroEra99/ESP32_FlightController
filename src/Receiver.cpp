/**
 * @file Receiver.cpp
 * @brief Implementazione della classe Receiver.
 *
 * Questo file contiene l'implementazione delle funzioni della classe Receiver,
 * utilizzata per leggere i dati da un ricevitore RC multi-canale IA6B.
 */

#include "Receiver.h"

/**
 * @brief Oggetto PWM per ciascun canale del ricevitore.
 *
 * Ogni membro rappresenta un canale PWM associato al ricevitore RC.
 */
PWMs pwm;

/**
 * @brief Costruttore della classe Receiver.
 *
 * Inizializza il ricevitore RC associando ciascun canale a un pin hardware
 * e configurando i corrispondenti oggetti PWM.
 *
 * @param receiver_pins Struttura contenente i pin associati ai canali del ricevitore.
 */
Receiver::Receiver(ReceiverPins receiver_pins)
{
    // Associazione dei pin ai membri della struttura ReceiverPins
    this->receiver_pins = receiver_pins;

    // Inizializzazione dei dati a zero
    data = {};

    // Associazione dei pin agli oggetti PWM e avvio del monitoraggio
    pwm.pwm_pitch.attach(receiver_pins.x);
    pwm.pwm_roll.attach(receiver_pins.y);
    pwm.pwm_throttle.attach(receiver_pins.throttle);
    pwm.pwm_yaw.attach(receiver_pins.z);
    pwm.pwm_swa.attach(receiver_pins.swa);
    pwm.pwm_swb.attach(receiver_pins.swb);
    pwm.pwm_swc.attach(receiver_pins.swc);
    pwm.pwm_swd.attach(receiver_pins.swd);
    pwm.pwm_vra.attach(receiver_pins.vra);
    pwm.pwm_vrb.attach(receiver_pins.vrb);

    pwm.pwm_pitch.begin(1);
    pwm.pwm_roll.begin(1);
    pwm.pwm_throttle.begin(1);
    pwm.pwm_yaw.begin(1);
    pwm.pwm_swa.begin(1);
    pwm.pwm_swb.begin(1);
    pwm.pwm_swc.begin(1);
    pwm.pwm_swd.begin(1);
    pwm.pwm_vra.begin(1);
    pwm.pwm_vrb.begin(1);
}

/**
 * @brief Legge i dati dai canali del ricevitore.
 *
 * Recupera i valori PWM da ciascun canale e li converte in una struttura `PilotData`
 * per l'elaborazione successiva.
 *
 * @return Struttura `PilotData` contenente i dati letti dai canali RC.
 */
PilotData Receiver::read()
{
    PilotData data;
    data.x = pwm.pwm_roll.getValue();
    data.y = pwm.pwm_pitch.getValue();
    data.throttle = pwm.pwm_throttle.getValue();
    data.z = pwm.pwm_yaw.getValue();
    data.swa = pwm.pwm_swa.getValue();
    data.swb = pwm.pwm_swb.getValue();
    data.swc = pwm.pwm_swc.getValue();
    data.swd = pwm.pwm_swd.getValue();
    data.vra = pwm.pwm_vra.getValue();
    data.vrb = pwm.pwm_vrb.getValue();
    return data;
}
