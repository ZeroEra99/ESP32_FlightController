/**
 * @class Receiver
 * @brief Implementazione di un ricevitore RC basato sul protocollo iBus.
 *
 * La classe legge i dati inviati dal ricevitore RC utilizzando il protocollo iBus
 * e li mappa in una struttura `PilotData`.
 */

#ifndef RECEIVER_H
#define RECEIVER_H

#include <Arduino.h>
#include "DataStructures.h"

class Receiver
{
private:
    HardwareSerial &serialPort; ///< Porta seriale utilizzata per la comunicazione iBus.
    PilotDataAnalog data;       ///< Dati letti dai canali del ricevitore.

    /**
     * @brief Decodifica un pacchetto iBus.
     *
     * Elabora il pacchetto ricevuto e aggiorna i dati del pilota.
     *
     * @param buffer Array contenente il pacchetto ricevuto.
     * @return true se il pacchetto è valido, false altrimenti.
     */
    bool decodeIBusPacket(const uint8_t *buffer);

public:
    /**
     * @brief Costruttore della classe Receiver.
     *
     * @param serial Porta seriale associata al ricevitore RC.
     */
    Receiver(HardwareSerial &serial);

    /**
     * @brief Legge i dati dal ricevitore RC.
     *
     * Legge i dati in formato iBus e li converte in una struttura `PilotData`.
     *
     * @return Struttura `PilotData` contenente i dati del pilota.
     */
    bool read(PilotDataAnalog &data);
};

#endif // RECEIVER_H
