/**
 * @file Utils.h
 * @brief Dichiarazioni delle funzioni di utilità per il progetto.
 *
 * Include funzioni matematiche per la gestione di quaternioni, conversioni di valori
 * tra formati digitali e PWM, e verifiche di intervalli.
 */

#ifndef UTILS_H
#define UTILS_H

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
    // Determiniamo il minimo e il massimo indipendentemente dall'ordine
    T lower = (min <= max) ? min : max;
    T upper = (min <= max) ? max : min;

    // Controlliamo se il valore è nel range
    return value >= lower && value <= upper;
}

/**
 * @brief Converte un valore digitale in un valore PWM.
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
 * @param q Quaternione di input.
 * @param q_conj Quaternione coniugato in output.
 */
void quaternion_conjugate(const float q[4], float q_conj[4]);

/**
 * @brief Moltiplica due quaternioni.
 *
 * @param q1 Primo quaternione.
 * @param q2 Secondo quaternione.
 * @param q_result Quaternione risultante.
 */
void quaternion_multiply(const float q1[4], const float q2[4], float q_result[4]);

/**
 * @brief Genera un quaternione da un asse e un angolo.
 *
 * @param axis Vettore dell'asse di rotazione.
 * @param angle_deg Angolo di rotazione in gradi.
 * @param q Quaternione generato.
 */
void quaternion_from_axis_angle(float axis[3], float angle_deg, float q[4]);

/**
 * @brief Normalizza un quaternione.
 *
 * @param q Quaternione da normalizzare.
 */
void quaternion_normalize(float q[4]);

#endif // UTILS_H
