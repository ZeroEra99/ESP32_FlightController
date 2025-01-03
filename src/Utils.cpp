/**
 * @file Utils.cpp
 * @brief Implementazione delle funzioni di utilità dichiarate in Utils.h.
 *
 * Questo file contiene funzioni utili per la gestione dei segnali PWM e dei quaternioni,
 * utilizzati nel controllo di volo.
 */

#include "Utils.h"
#include <cmath>

/**
 * @brief Converte un valore digitale in un valore PWM.
 *
 * Applica una mappatura lineare per convertire un valore digitale in un valore PWM,
 * rispettando i limiti specificati per il range analogico.
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
    int pwm_value = (int)((value - min_digital) * (max_analog - min_analog) / (max_digital - min_digital) + min_analog);

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
 * Applica una mappatura lineare per convertire un valore PWM in un valore digitale,
 * rispettando i limiti specificati per il range digitale.
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
 * @brief Calcola il coniugato di un quaternione.
 *
 * Il coniugato di un quaternione è calcolato invertendo il segno delle componenti vettoriali.
 *
 * @param q Quaternione di input.
 * @param q_conj Quaternione coniugato in output.
 */
void quaternion_conjugate(const Quaternion &q, Quaternion &q_conj)
{
    q_conj.w = q.w;
    q_conj.x = -q.x;
    q_conj.y = -q.y;
    q_conj.z = -q.z;
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
void quaternion_multiply(const Quaternion &q1, const Quaternion &q2, Quaternion &q_result)
{
    q_result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    q_result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    q_result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    q_result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
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
void quaternion_from_axis_angle(const float axis[3], float angle_deg, Quaternion &q)
{
    float angle_rad = angle_deg * M_PI / 180.0f;
    float sin_half_angle = sin(angle_rad / 2);
    q.w = cos(angle_rad / 2);
    q.x = axis[0] * sin_half_angle;
    q.y = axis[1] * sin_half_angle;
    q.z = axis[2] * sin_half_angle;
}

/**
 * @brief Normalizza un quaternione.
 *
 * Divide ogni componente del quaternione per la sua magnitudine per assicurare
 * che rappresenti una rotazione valida.
 *
 * @param q Quaternione da normalizzare.
 */
void quaternion_normalize(Quaternion &q)
{
    float magnitude = sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);

    if (magnitude > 0.0f)
    {
        q.w /= magnitude;
        q.x /= magnitude;
        q.y /= magnitude;
        q.z /= magnitude;
    }
}
