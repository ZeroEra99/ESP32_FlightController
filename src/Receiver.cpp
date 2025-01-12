#include "Receiver.h"
// #include "DebugLogger.h"
#include "HardwareParameters.h"

double pwm_to_digital(int pwm_value, int min_analog, int max_analog, double min_digital, double max_digital)
{
    double digital_value = (double)(pwm_value - min_analog) / (max_analog - min_analog) * (max_digital - min_digital) + min_digital;

    if (digital_value < min_digital)
    {
        digital_value = min_digital;
    }
    else if (digital_value > max_digital)
    {
        digital_value = max_digital;
    }

    return digital_value;
}

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

Receiver::Receiver(int rxPin)
{
    // Configura UART su Serial1
    Serial1.begin(115200, SERIAL_8N1, rxPin, -1); // RX su rxPin, nessun TX
    if (check_receiver(2000)) // Timeout di 2 secondi
    {
        Serial.println("Receiver setup complete.");
    }
    else
    {
        Serial.println("Receiver setup failed.");
    }

    dataRAW = {0}; // Inizializza i dati RAW
}

bool Receiver::check_receiver(unsigned long timeout)
{
    unsigned long start_time = millis();
    while (millis() - start_time < timeout)
    {
        if (Serial1.available() > 0) // Dati ricevuti sulla porta
        {
            Serial.println("Receiver detected.");
            return true;
        }
    }
    Serial.println("Receiver not detected within timeout.");
    return false;
}

bool Receiver::decodeIBusPacket(const uint8_t *buffer)
{
    // Verifica l'header del pacchetto
    if (buffer[0] != 0x20 || buffer[1] != 0x40)
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

void Receiver::logData(const ReceiverData &data)
{
    // Log semplificato per Arduino Serial Monitor
    Serial.print("X: "); Serial.print(data.x);
    Serial.print(" Y: "); Serial.print(data.y);
    Serial.print(" Z: "); Serial.print(data.z);
    Serial.print(" Throttle: "); Serial.print(data.throttle);
    Serial.print(" Switch A: "); Serial.print(data.swa);
    Serial.print(" Switch B: "); Serial.print(data.swb);
    Serial.print(" VRA: "); Serial.print(data.vra);
    Serial.print(" VRB: "); Serial.println(data.vrb);
    Serial.print("\n");
}

bool Receiver::read(ReceiverData &data)
{
    // Legge e decodifica un pacchetto iBus
    static uint8_t buffer[32]; // Buffer per un pacchetto iBus
    if (Serial1.available() >= 32)
    {
        Serial1.readBytes(buffer, 32); // Legge 32 byte
        if (decodeIBusPacket(buffer))
        {
            // Converte i dati RAW nei valori normalizzati per il sistema
            data.x = pwm_to_digital(dataRAW.ch1, PWM_MIN, PWM_MAX, ROLL_MIN, ROLL_MAX);
            data.y = pwm_to_digital(dataRAW.ch2, PWM_MIN, PWM_MAX, PITCH_MIN, PITCH_MAX);
            data.throttle = pwm_to_digital(dataRAW.ch3, PWM_MIN, PWM_MAX, THROTTLE_MIN, THROTTLE_MAX);
            data.z = pwm_to_digital(dataRAW.ch4, PWM_MIN, PWM_MAX, YAW_MIN, YAW_MAX);
            data.swa = pwm_to_digital(dataRAW.ch5, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_ABC_MAX);
            data.swb = pwm_to_digital(dataRAW.ch6, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_ABC_MAX);
            data.swc =0; //pwm_to_digital(dataRAW.ch7, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_ABC_MAX);
            data.swd =0; //pwm_to_digital(dataRAW.ch8, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_D_MAX);
            data.vra =0; //pwm_to_digital(dataRAW.ch9, PWM_MIN, PWM_MAX, VRA_MIN, VRA_MAX);
            data.vrb =0; //pwm_to_digital(dataRAW.ch10, PWM_MIN, PWM_MAX, VRB_MIN, VRB_MAX);
            return true; // Conferma la validità dei dati
        }
    }
    return false; // Nessun dato valido
}
