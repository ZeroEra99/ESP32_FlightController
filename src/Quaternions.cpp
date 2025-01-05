#include "Quaternions.h"

/**
 * @brief Calcola il coniugato di un quaternione.
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
 */
void quaternion_multiply(const Quaternion &q1, const Quaternion &q2, Quaternion &q_result)
{
    q_result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    q_result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    q_result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    q_result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
}

/**
 * @brief Converte un asse e un angolo in un quaternione.
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
 * @brief Normalizza un quaternione per renderlo unitario.
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

/**
 * @brief Compone una serie di quaternioni in un unico quaternione risultante.
 */
void quaternion_compose(const Quaternion *quaternions, size_t count, Quaternion &result)
{
    result = {1, 0, 0, 0}; // Quaternione unitario
    for (size_t i = 0; i < count; ++i)
    {
        Quaternion temp;
        quaternion_multiply(result, quaternions[i], temp);
        result = temp;
    }
}

/**
 * @brief Calcola l'errore tra un quaternione desiderato e quello attuale.
 */
void quaternion_error(const Quaternion &desired, const Quaternion &actual, Quaternion &error)
{
    Quaternion conjugate;
    quaternion_conjugate(actual, conjugate);
    quaternion_multiply(desired, conjugate, error);
}
