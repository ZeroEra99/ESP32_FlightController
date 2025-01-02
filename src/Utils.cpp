#include "Utils.h"
#include <cmath>

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

void quaternion_conjugate(const float q[4], float q_conj[4])
{
    q_conj[0] = q[0];
    q_conj[1] = -q[1];
    q_conj[2] = -q[2];
    q_conj[3] = -q[3];
}

void quaternion_multiply(const float q1[4], const float q2[4], float q_result[4])
{
    q_result[0] = q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3];
    q_result[1] = q1[0] * q2[1] + q1[1] * q2[0] + q1[2] * q2[3] - q1[3] * q2[2];
    q_result[2] = q1[0] * q2[2] - q1[1] * q2[3] + q1[2] * q2[0] + q1[3] * q2[1];
    q_result[3] = q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1] + q1[3] * q2[0];
}

void quaternion_from_axis_angle(float axis[3], float angle_deg, float q[4])
{
    float angle_rad = angle_deg * M_PI / 180.0; // Converti gradi in radianti
    float sin_half_angle = sin(angle_rad / 2);
    q[0] = cos(angle_rad / 2);          // Componente scalare
    q[1] = axis[0] * sin_half_angle;    // Componente x
    q[2] = axis[1] * sin_half_angle;    // Componente y
    q[3] = axis[2] * sin_half_angle;    // Componente z
}

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
