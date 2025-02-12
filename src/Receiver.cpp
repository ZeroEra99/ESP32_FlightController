#include "Receiver.h"
#include "Logger.h"

Receiver::Receiver(int rxPin) : rxPin(rxPin), bufferIndex(0)
{
    Serial1.begin(115200, SERIAL_8N1, rxPin, -1); // Configura UART solo per RX
    resetBuffer();
    Logger::getInstance().log(LogLevel::INFO, "Receiver setup complete.");
}

void Receiver::resetBuffer()
{
    bufferIndex = 0;
    memset(buffer, 0, PACKET_SIZE);
}

bool Receiver::decodeIBusPacket()
{
    if (buffer[0] != HEADER_1 || buffer[1] != HEADER_2)
        return false;

    // Calcola il checksum
    uint16_t checksum = 0xFFFF;
    for (int i = 0; i < 30; ++i)
    {
        checksum -= buffer[i];
    }

    uint16_t receivedChecksum = buffer[30] | (buffer[31] << 8);
    if (checksum != receivedChecksum)
        return false;

    return true;
}

/**
 * @brief Converte un valore PWM in un valore normalizzato digitale.
 * 
 * @param pwm_value Valore PWM ricevuto.
 * @param min_analog Valore minimo PWM.
 * @param max_analog Valore massimo PWM.
 * @param min_digital Valore minimo normalizzato.
 * @param max_digital Valore massimo normalizzato.
 * 
 * @return double Valore normalizzato.
 */
double pwmToDigital(int pwm_value, int min_analog, int max_analog, double min_digital, double max_digital)
{
    double digital_value = ((double)(pwm_value - min_analog) / (max_analog - min_analog)) * (max_digital - min_digital) + min_digital;
    return (digital_value < min_digital) ? min_digital : (digital_value > max_digital) ? max_digital
                                                                                       : digital_value;
}

bool Receiver::read(ReceiverData &data)
{
    while (Serial1.available() > 0)
    {
        size_t bytesToRead = Serial1.available();
        if (bytesToRead > PACKET_SIZE - bufferIndex)
            bytesToRead = PACKET_SIZE - bufferIndex; // Non superare lo spazio disponibile nel buffer

        size_t bytesRead = Serial1.readBytes(buffer + bufferIndex, bytesToRead);
        bufferIndex += bytesRead;

        // Gestire pacchetti completi
        while (bufferIndex >= PACKET_SIZE)
        {
            if (decodeIBusPacket())
            {
                // Decodifica dati
                for (int i = 0; i < 10; ++i)
                {
                    int16_t pwm_value = buffer[2 + i * 2] | (buffer[3 + i * 2] << 8);
                    if (pwm_value < 0)
                    {
                        resetBuffer(); // Ripristina il buffer in caso di valori errati
                        return false;
                    }

                    switch (i)
                    {
                    case 0:
                        data.x = pwmToDigital(pwm_value, PWM_MIN, PWM_MAX, ROLL_MIN, ROLL_MAX);
                        break;
                    case 1:
                        data.y = pwmToDigital(pwm_value, PWM_MIN, PWM_MAX, PITCH_MIN, PITCH_MAX);
                        break;
                    case 2:
                        data.throttle = pwmToDigital(pwm_value, PWM_MIN, PWM_MAX, THROTTLE_MIN, THROTTLE_MAX);
                        break;
                    case 3:
                        data.z = pwmToDigital(pwm_value, PWM_MIN, PWM_MAX, YAW_MIN, YAW_MAX);
                        break;
                    case 4:
                        data.swa = pwmToDigital(pwm_value, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_ABD_MAX);
                        break;
                    case 5:
                        data.swb = pwmToDigital(pwm_value, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_ABD_MAX);
                        break;
                    case 6:
                        data.swc = pwmToDigital(pwm_value, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_C_MAX);
                        break;
                    case 7:
                        data.swd = pwmToDigital(pwm_value, PWM_MIN, PWM_MAX, SWITCH_MIN, SWITCH_SW_ABD_MAX);
                        break;
                    case 8:
                        data.vra = pwmToDigital(pwm_value, PWM_MIN, PWM_MAX, VRA_MIN, VRA_MAX);
                        break;
                    case 9:
                        data.vrb = pwmToDigital(pwm_value, PWM_MIN, PWM_MAX, VRB_MIN, VRB_MAX);
                        break;
                    }
                }

                resetBuffer();
                return true;
            }
            else
            {
                // Rimuovere il primo byte dal buffer e spostare gli altri
                memmove(buffer, buffer + 1, --bufferIndex);
            }
        }
    }

    // Se il buffer non contiene pacchetti validi, resettalo
    resetBuffer();
    return false; // Nessun pacchetto valido letto
}

void Receiver::logData(const ReceiverData &data)
{
    Logger::getInstance().logData("x", data.x);
    Logger::getInstance().logData("y", data.y);
    Logger::getInstance().logData("throttle", data.throttle);
    Logger::getInstance().logData("z", data.z);
    Logger::getInstance().logData("swa", data.swa);
    Logger::getInstance().logData("swb", data.swb);
    Logger::getInstance().logData("swc", data.swc);
    Logger::getInstance().logData("swd", data.swd);
    Logger::getInstance().logData("vra", data.vra);
    Logger::getInstance().logData("vrb", data.vrb);
}
