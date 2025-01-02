#ifndef UTILS_H
#define UTILS_H

// Matrice di rotazione per la conversione da quaternioni ad angoli di Eulero
static float axis[3][3] = {
    {1, 0, 0}, // Asse X (ROLL)
    {0, 1, 0}, // Asse Y (PITCH)
    {0, 0, 1}  // Asse Z (YAW)
};


template <typename T, typename U, typename V>
bool isInRange(T value, U min, V max)
{
    // Determiniamo il minimo e il massimo indipendentemente dall'ordine
    T lower = (min <= max) ? min : max;
    T upper = (min <= max) ? max : min;

    // Controlliamo se il valore è nel range
    return value >= lower && value <= upper;
}

int digital_to_pwm(double value, double min_digital, double max_digital, int min_analog, int max_analog);
double pwm_to_digital(int pwm_value, int min_analog, int max_analog, double min_digital, double max_digital);
void quaternion_conjugate(const float q[4], float q_conj[4]);
void quaternion_multiply(const float q1[4], const float q2[4], float q_result[4]);
void quaternion_from_axis_angle(float axis[3], float angle_deg, float q[4]);
void quaternion_normalize(float q[4]);

#endif // UTILS_H