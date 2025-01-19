/**
 * @file Receiver.h
 * @brief Dichiarazione della classe Receiver per la gestione del ricevitore IBUS.
 */
#ifndef RECEIVER_H
#define RECEIVER_H

#include <Arduino.h>
#include "HardwareParameters.h"
#include "DataStructures.h"

/**
 * @brief Struttura dati per i dati ricevuti dal ricevitore.
 */
struct ReceiverDataRAW
{
    int16_t ch1;  ///< Rollio.
    int16_t ch2;  ///< Beccheggio.
    int16_t ch3;  ///< Throttle.
    int16_t ch4;  ///< Yaw.
    int16_t ch5;  ///< Switch A.
    int16_t ch6;  ///< Switch B.
    int16_t ch7;  ///< Switch C.
    int16_t ch8;  ///< Switch D.
    int16_t ch9;  ///< Potenziometro A.
    int16_t ch10; ///< Potenziometro B.
};

/**
 * @brief Classe per la gestione del ricevitore IBUS.
 *
 * Questa classe permette di leggere i dati inviati dal ricevitore IBUS e decodificarli.
 * I dati vengono memorizzati in una struttura `ReceiverData` per essere utilizzati dal sistema.
 *
 * La classe è implementata in modo da essere non bloccante, permettendo di leggere i dati
 * in modo asincrono rispetto al ciclo principale.
 */
class Receiver
{
private:
    static const int PACKET_SIZE = 32;    ///< Dimensione massima del pacchetto IBUS.
    static const uint8_t HEADER_1 = 0x20; ///< Primo byte dell'header del pacchetto IBUS.
    static const uint8_t HEADER_2 = 0x40; ///< Secondo byte dell'header del pacchetto IBUS.

    int rxPin;                   ///< Pin di ricezione del segnale IBUS.
    uint8_t buffer[PACKET_SIZE]; ///< Buffer per la memorizzazione del pacchetto IBUS.
    size_t bufferIndex;          ///< Indice corrente del buffer.

    /**
     * @brief Verifica e decodifica un pacchetto IBUS dal buffer.
     *
     * @return true Se il pacchetto è valido.
     * @return false Se il pacchetto è invalido.
     */
    bool decodeIBusPacket();

    /**
     * @brief Resetta il buffer del ricevitore.
     */
    void resetBuffer();

public:
    /**
     * @brief Costruttore della classe Receiver.
     *
     * Inizializza la comunicazione seriale per la lettura dei dati.
     *
     * @param rxPin Pin di ricezione del segnale IBUS.
     */
    Receiver(int rxPin);

    /**
     * @brief Legge i dati dal ricevitore IBUS.
     *
     * @param data Riferimento alla struttura `ReceiverData` per memorizzare i dati letti.
     * @return true Se i dati sono stati letti correttamente.
     * @return false Se si è verificato un errore durante la lettura.
     */
    bool read(ReceiverData &data);

    /**
     * @brief Logga i dati ricevuti dal ricevitore.
     *
     * @param data Dati ricevuti dal ricevitore.
     */
    void logData(const ReceiverData &data);
};

#endif // RECEIVER_H
