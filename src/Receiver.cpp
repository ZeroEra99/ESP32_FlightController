/**
 * @file Receiver.cpp
 * @brief Implementazione della classe Receiver.
 *
 * Questo file contiene l'implementazione delle funzioni della classe Receiver,
 * utilizzata per leggere i dati da un ricevitore RC multi-canale IA6B.
 */

#include "Receiver.h"

/**
 * @brief Costruttore della classe Receiver.
 *
 * Inizializza il ricevitore iBus collegandosi a una porta seriale.
 *
 * @param serial Porta seriale associata al ricevitore RC.
 */
Receiver::Receiver(HardwareSerial &serial) : serialPort(serial)
{
    serialPort.begin(115200); // Velocità di trasmissione iBus
    data = {0};               // Inizializza i dati del pilota
}

/**
 * @brief Decodifica un pacchetto iBus.
 *
 * @param buffer Array contenente il pacchetto ricevuto.
 * @return true se il pacchetto è valido, false altrimenti.
 */
bool Receiver::decodeIBusPacket(const uint8_t *buffer)
{
    // Verifica l'header del pacchetto
    if (buffer[0] != 0x20)
        return false;

    // Calcolo del checksum
    uint16_t checksum = 0xFFFF;
    for (int i = 0; i < 30; i++) // 30 byte di dati
    {
        checksum -= buffer[i];
    }

    // Confronta il checksum
    uint16_t receivedChecksum = (buffer[30] | (buffer[31] << 8));
    if (checksum != receivedChecksum)
        return false;

    // Aggiorna i dati del pilota
    for (int i = 0; i < 10; i++) // I primi 10 canali
    {
        ((int16_t *)&data)[i] = buffer[2 + i * 2] | (buffer[3 + i * 2] << 8);
    }

    return true;
}

/**
 * @brief Legge i dati dal ricevitore RC.
 *
 * @return Struttura `PilotData` contenente i dati del pilota.
 */
bool Receiver::read(PilotDataAnalog &pilot_data_analog)
{
    static uint8_t buffer[32]; // Buffer per un pacchetto iBus
    if (serialPort.available() >= 32)
    {
        serialPort.readBytes(buffer, 32); // Legge 32 byte
        if (decodeIBusPacket(buffer))
        {
            pilot_data_analog = data; // Ritorna i dati validi
            return true;              // Conferma la validità
        }
    }
    return false; // Nessun dato valido
}
