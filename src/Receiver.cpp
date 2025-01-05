#include "Receiver.h"
#include "DebugLogger.h"
#include "HardwareParameters.h"
/**
 * @brief Struttura per contenere i dati RAW ricevuti dal protocollo iBus.
 */
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

ReceiverDataRAW dataRAW;

Receiver::Receiver(HardwareSerial &serial) : serialPort(serial)
{
    // Configura la porta seriale per il ricevitore
    serialPort.begin(115200); // Velocità di trasmissione iBus
    dataRAW = {0};            // Inizializza i dati RAW
    DebugLogger::getInstance()->log("Receiver setup complete.", LogLevel::DEBUG);
}

bool Receiver::decodeIBusPacket(const uint8_t *buffer)
{
    // Verifica l'header del pacchetto
    if (buffer[0] != 0x20)
        return false;

    // Calcola il checksum del pacchetto
    uint16_t checksum = 0xFFFF;
    for (int i = 0; i < 30; i++) // 30 byte di dati
    {
        checksum -= buffer[i];
    }

    // Confronta il checksum ricevuto
    uint16_t receivedChecksum = (buffer[30] | (buffer[31] << 8));
    if (checksum != receivedChecksum)
        return false;

    // Aggiorna i dati RAW ricevuti dal pilota
    for (int i = 0; i < 10; i++) // I primi 10 canali
    {
        ((int16_t *)&dataRAW)[i] = buffer[2 + i * 2] | (buffer[3 + i * 2] << 8);
    }

    return true;
}

bool Receiver::read(ReceiverData &data)
{
    // Legge e decodifica un pacchetto iBus
    static uint8_t buffer[32]; // Buffer per un pacchetto iBus
    if (serialPort.available() >= 32)
    {
        serialPort.readBytes(buffer, 32); // Legge 32 byte
        if (decodeIBusPacket(buffer))
        {
            // Converte i dati RAW nei valori normalizzati per il sistema
            data.x = map(dataRAW.ch1, PWM_MIN, PWM_MAX, ROLL_MIN, ROLL_MAX);
            data.y = map(dataRAW.ch2, PWM_MIN, PWM_MAX, PITCH_MIN, PITCH_MAX);
            data.throttle = map(dataRAW.ch3, PWM_MIN, PWM_MAX, THROTTLE_MIN, THROTTLE_MAX);
            data.z = map(dataRAW.ch4, PWM_MIN, PWM_MAX, YAW_MIN, YAW_MAX);
            data.swa = map(dataRAW.ch5, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_ABC_MAX);
            data.swb = map(dataRAW.ch6, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_ABC_MAX);
            data.swc = map(dataRAW.ch7, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_ABC_MAX);
            data.swd = map(dataRAW.ch8, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_D_MAX);
            data.vra = map(dataRAW.ch9, PWM_MIN, PWM_MAX, VRA_MIN, VRA_MAX);
            data.vrb = map(dataRAW.ch10, PWM_MIN, PWM_MAX, VRB_MIN, VRB_MAX);
            return true; // Conferma la validità dei dati
        }
    }
    return false; // Nessun dato valido
}
