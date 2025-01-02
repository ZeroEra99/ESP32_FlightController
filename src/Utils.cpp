/**
 * @file Utils.cpp
 * @brief Implementazione delle funzioni di utilità dichiarate in Utils.h.
 */

#include "Utils.h"
#include <cmath>

/**
 * @brief Converte un valore digitale in un valore PWM.
 * 
 * Applica una mappatura lineare per convertire un valore digitale in un valore PWM, rispettando
 * i limiti specificati per il range analogico.
 * 
 * @param value Valore digitale da convertire.
 * @param min_digital Valore minimo del range digitale.
 * @param max_digital Valore massimo del range digitale.
 * @param min_analog Valore minimo del range analogico (PWM).
 * @param max_analog Valore massimo del range analogico (PWM).
 * @return Valore convertito in formato PWM.
 */
int digital_to_pwm(double value, double min_digital, double max_digital, int min_analog, int max_analog)
{
    // Formula di mappatura lineare per ottenere il valore PWM
    int pwm_value = (int)((value - min_digital) * (max_analog - min_analog) / (max_digital - min_digital) + min_analog);

    // Verifica che il valore sia compreso nell'intervallo analogico
    if (pwm_value < min_analog)
    {
        pwm_value = min_analog;
    }
    else if (pwm_value > max_analog)
    {
        pwm_value = max_analog;
    }

    return pwm_value;
}

/**
 * @brief Converte un valore PWM in un valore digitale.
 * 
 * Applica una mappatura lineare per convertire un valore PWM in un valore digitale, rispettando
 * i limiti specificati per il range digitale.
 * 
 * @param pwm_value Valore PWM da convertire.
 * @param min_analog Valore minimo del range analogico (PWM).
 * @param max_analog Valore massimo del range analogico (PWM).
 * @param min_digital Valore minimo del range digitale.
 * @param max_digital Valore massimo del range digitale.
 * @return Valore convertito in formato digitale.
 */
double pwm_to_digital(int pwm_value, int min_analog, int max_analog, double min_digital, double max_digital)
{
    // Mappatura lineare del valore PWM nel range digitale
    double digital_value = (double)(pwm_value - min_analog) / (max_analog - min_analog) * (max_digital - min_digital) + min_digital;
    // Verifica che il valore sia compreso nell'intervallo digitale
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
 * @brief Calcola il coniugato di un quaternione.
 * 
 * Il coniugato di un quaternione è calcolato invertendo il segno delle componenti vettoriali.
 * 
 * @param q Quaternione di input.
 * @param q_conj Quaternione coniugato in output.
 */
void quaternion_conjugate(const float q[4], float q_conj[4])
{
    q_conj[0] = q[0];
    q_conj[1] = -q[1];
    q_conj[2] = -q[2];
    q_conj[3] = -q[3];
}

/**
 * @brief Moltiplica due quaternioni.
 * 
 * Esegue la moltiplicazione tra due quaternioni, utile per comporre rotazioni.
 * 
 * @param q1 Primo quaternione.
 * @param q2 Secondo quaternione.
 * @param q_result Quaternione risultante.
 */
void quaternion_multiply(const float q1[4], const float q2[4], float q_result[4])
{
    q_result[0] = q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3];
    q_result[1] = q1[0] * q2[1] + q1[1] * q2[0] + q1[2] * q2[3] - q1[3] * q2[2];
    q_result[2] = q1[0] * q2[2] - q1[1] * q2[3] + q1[2] * q2[0] + q1[3] * q2[1];
    q_result[3] = q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1] + q1[3] * q2[0];
}

/**
 * @brief Genera un quaternione da un asse e un angolo.
 * 
 * Converte un asse di rotazione e un angolo in gradi in un quaternione.
 * 
 * @param axis Vettore dell'asse di rotazione.
 * @param angle_deg Angolo di rotazione in gradi.
 * @param q Quaternione generato.
 */
void quaternion_from_axis_angle(float axis[3], float angle_deg, float q[4])
{
    float angle_rad = angle_deg * M_PI / 180.0; // Converti gradi in radianti
    float sin_half_angle = sin(angle_rad / 2);
    q[0] = cos(angle_rad / 2);          // Componente scalare
    q[1] = axis[0] * sin_half_angle;    // Componente x
    q[2] = axis[1] * sin_half_angle;    // Componente y
    q[3] = axis[2] * sin_half_angle;    // Componente z
}

/**
 * @brief Normalizza un quaternione.
 * 
 * Divide ogni componente del quaternione per la sua magnitudine per assicurare
 * che rappresenti una rotazione valida.
 * 
 * @param q Quaternione da normalizzare.
 */
void quaternion_normalize(float q[4])
{
    // Calcola la magnitudine del quaternione
    float magnitude = sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);

    // Evita la divisione per zero
    if (magnitude > 0.0f)
    {
        for (int i = 0; i < 4; i++)
        {
            q[i] /= magnitude;
        }
    }
}
