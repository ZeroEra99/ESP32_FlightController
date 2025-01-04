/**
 * @file Utils.h
 * @brief Dichiarazioni delle funzioni di utilità per il progetto.
 *
 * Include funzioni matematiche per la gestione di quaternioni, conversioni di valori
 * tra formati digitali e PWM, e verifiche di intervalli.
 */

#ifndef UTILS_H
#define UTILS_H

#include "DataStructures.h"

/**
 * @brief Matrice di rotazione per la conversione da quaternioni ad angoli di Eulero.
 *
 * Ogni riga rappresenta un asse cartesiano:
 * - X: Roll
 * - Y: Pitch
 * - Z: Yaw
 */
static float axis[3][3] = {
    {1, 0, 0}, // Asse X (ROLL)
    {0, 1, 0}, // Asse Y (PITCH)
    {0, 0, 1}  // Asse Z (YAW)
};

/**
 * @brief Controlla se un valore è compreso in un intervallo specificato.
 *
 * Funzione generica per verificare se un valore si trova tra due limiti,
 * indipendentemente dall'ordine di `min` e `max`.
 *
 * @tparam T Tipo del valore da verificare.
 * @tparam U Tipo del limite minimo.
 * @tparam V Tipo del limite massimo.
 * @param value Il valore da verificare.
 * @param min Il limite minimo dell'intervallo.
 * @param max Il limite massimo dell'intervallo.
 * @return true se il valore è compreso nell'intervallo, false altrimenti.
 */
template <typename T, typename U, typename V>
bool isInRange(T value, U min, V max)
{
    T lower = (min <= max) ? min : max;
    T upper = (min <= max) ? max : min;

    return value >= lower && value <= upper;
}

/**
 * @brief Converte un valore digitale in un valore PWM.
 *
 * Esegue una mappatura lineare tra un intervallo digitale e un intervallo PWM.
 *
 * @param value Valore digitale da convertire.
 * @param min_digital Valore minimo del range digitale.
 * @param max_digital Valore massimo del range digitale.
 * @param min_analog Valore minimo del range analogico (PWM).
 * @param max_analog Valore massimo del range analogico (PWM).
 * @return Valore convertito in formato PWM.
 */
int digital_to_pwm(double value, double min_digital, double max_digital, int min_analog, int max_analog);

/**
 * @brief Converte un valore PWM in un valore digitale.
 *
 * Esegue una mappatura lineare tra un intervallo PWM e un intervallo digitale.
 *
 * @param pwm_value Valore PWM da convertire.
 * @param min_analog Valore minimo del range analogico (PWM).
 * @param max_analog Valore massimo del range analogico (PWM).
 * @param min_digital Valore minimo del range digitale.
 * @param max_digital Valore massimo del range digitale.
 * @return Valore convertito in formato digitale.
 */
double pwm_to_digital(int pwm_value, int min_analog, int max_analog, double min_digital, double max_digital);

/**
 * @brief Calcola il coniugato di un quaternione.
 *
 * Il coniugato di un quaternione è ottenuto invertendo il segno delle componenti vettoriali,
 * mentre la componente scalare rimane invariata.
 *
 * @param q Quaternione di input.
 * @param q_conj Quaternione coniugato in output.
 */
void quaternion_conjugate(const Quaternion &q, Quaternion &q_conj);

/**
 * @brief Moltiplica due quaternioni.
 *
 * Esegue la moltiplicazione tra due quaternioni, utile per comporre rotazioni nello spazio tridimensionale.
 *
 * @param q1 Primo quaternione.
 * @param q2 Secondo quaternione.
 * @param q_result Quaternione risultante.
 */
void quaternion_multiply(const Quaternion &q1, const Quaternion &q2, Quaternion &q_result);

/**
 * @brief Genera un quaternione da un asse e un angolo.
 *
 * Converte un asse di rotazione e un angolo in gradi in un quaternione corrispondente.
 *
 * @param axis Vettore dell'asse di rotazione.
 * @param angle_deg Angolo di rotazione in gradi.
 * @param q Quaternione generato.
 */
void quaternion_from_axis_angle(float axis[3], float angle_deg, Quaternion &q);

/**
 * @brief Normalizza un quaternione.
 *
 * Divide ogni componente del quaternione per la sua magnitudine per assicurare
 * che rappresenti una rotazione valida (magnitudine uguale a 1).
 *
 * @param q Quaternione da normalizzare.
 */
void quaternion_normalize(Quaternion &q);

/**
 * @brief Compone una rotazione da più quaternioni.
 *
 * Moltiplica una serie di quaternioni in ordine per generare
 * una rotazione composta.
 *
 * @param quaternions Array di quaternioni da moltiplicare.
 * @param count Numero di quaternioni nell'array.
 * @param result Quaternione risultante.
 */
void quaternion_compose(const Quaternion* quaternions, size_t count, Quaternion& result);

/**
 * @brief Calcola l'errore tra due quaternioni.
 *
 * Calcola l'errore come prodotto tra il quaternione desiderato
 * e il coniugato del quaternione attuale.
 *
 * @param desired Quaternione desiderato.
 * @param actual Quaternione attuale.
 * @param error Quaternione risultante come errore.
 */
void quaternion_error(const Quaternion &desired, const Quaternion &actual, Quaternion &error);

#endif // UTILS_H
