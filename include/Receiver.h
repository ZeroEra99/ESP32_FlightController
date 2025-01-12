/**
 * @file Receiver.h
 * @brief Dichiarazione della classe Receiver per la gestione del protocollo iBus.
 *
 * Questa classe consente di leggere e decodificare i dati ricevuti dal pilota tramite
 * il protocollo iBus e di convertirli in un formato utilizzabile dal sistema.
 */

#ifndef RECEIVER_H
#define RECEIVER_H

#include <Arduino.h>
#include "DataStructures.h"

/**
 * @brief Classe per la gestione del ricevitore.
 *
 * Implementa la logica per leggere e decodificare i dati ricevuti dal pilota.
 */
class Receiver
{
private:
    /**
     * @brief Verifica la presenza di dati sulla porta seriale.
     * 
     * @param timeout Timeout massimo per la verifica.
     * @return true Se sono presenti dati sulla porta seriale.
     * @return false Se non sono presenti dati sulla porta seriale.
     */
    bool check_receiver(unsigned long timeout);

    /**
     * @brief Decodifica un pacchetto ricevuto dal protocollo iBus.
     *
     * @param buffer Buffer contenente i dati del pacchetto iBus.
     * @return true Se il pacchetto è valido.
     * @return false Se il pacchetto non è valido.
     */
    bool decodeIBusPacket(const uint8_t *buffer);

public:
    /**
     * @brief Costruttore della classe Receiver.
     *
     * @param rxPin Pin RX da utilizzare per il protocollo iBus.
     */
    Receiver(int rxPin);

    /**
     * @brief Salva i dati ricevuti dal pilota.
     *
     * @param data Dati ricevuti dal pilota.
     */
    void logData(const ReceiverData &data);

    /**
     * @brief Legge e converte i dati ricevuti dal protocollo iBus.
     *
     * @param data Struttura in cui vengono salvati i dati ricevuti.
     * @return true Se la lettura e la conversione dei dati hanno avuto successo.
     * @return false Se si è verificato un errore durante la lettura o la decodifica.
     */
    bool read(ReceiverData &data);
};

#endif // RECEIVER_H
