// Receiver.h
#ifndef RECEIVER_H
#define RECEIVER_H

#include <Arduino.h>
#include "HardwareParameters.h" // Inclusione delle macro per i parametri
#include "DataStructures.h"

struct ReceiverDataRAW
{
    int16_t ch1;
    int16_t ch2;
    int16_t ch3;
    int16_t ch4;
    int16_t ch5;
    int16_t ch6;
    int16_t ch7;
    int16_t ch8;
    int16_t ch9;
    int16_t ch10;
};

class Receiver
{
private:
    static const int PACKET_SIZE = 32;
    static const uint8_t HEADER_1 = 0x20;
    static const uint8_t HEADER_2 = 0x40;

    int rxPin;
    uint8_t buffer[PACKET_SIZE];
    size_t bufferIndex;

    /**
     * @brief Verifica e decodifica un pacchetto IBUS dal buffer.
     *
     * @return true Se il pacchetto è valido.
     * @return false Se il pacchetto è invalido.
     */
    bool decodeIBusPacket();

    /**
     * @brief Converte un valore PWM in un valore normalizzato digitale.
     *
     * @param pwm_value Valore PWM ricevuto.
     * @param min_analog Valore minimo PWM.
     * @param max_analog Valore massimo PWM.
     * @param min_digital Valore minimo normalizzato.
     * @param max_digital Valore massimo normalizzato.
     * @return double Valore normalizzato.
     */
    double pwmToDigital(int pwm_value, int min_analog, int max_analog, double min_digital, double max_digital);

    void resetBuffer();

public:
    Receiver(int rxPin);

    /**
     * @brief Esegue la lettura non bloccante dei dati del ricevitore.
     *
     * @param data Oggetto ReceiverData per salvare i dati decodificati.
     * @return true Se i dati sono stati letti e decodificati correttamente.
     * @return false Se i dati non sono disponibili o sono invalidi.
     */
    bool read(ReceiverData &data);

    /**
     * @brief Logga i dati ricevuti sul monitor seriale.
     *
     * @param data Oggetto ReceiverData contenente i dati da loggare.
     */
    void logData(const ReceiverData &data);
};

#endif // RECEIVER_H
