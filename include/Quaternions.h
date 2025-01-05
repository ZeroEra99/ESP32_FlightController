/**
 * @file Quaternions.h
 * @brief Dichiarazione delle funzioni per la manipolazione dei quaternioni.
 * 
 * Questo file include le funzioni necessarie per il calcolo e la gestione dei quaternioni,
 * inclusi la moltiplicazione, la normalizzazione e il calcolo degli errori.
 */

#ifndef QUATERNIONS_H
#define QUATERNIONS_H

#include "DataStructures.h"
#include <cmath>

/**
 * @brief Array statico di assi standard per i calcoli dei quaternioni.
 */
static float axis[3][3] = {
    {1, 0, 0}, // Asse X (ROLL)
    {0, 1, 0}, // Asse Y (PITCH)
    {0, 0, 1}  // Asse Z (YAW)
};

/**
 * @brief Calcola il coniugato di un quaternione.
 * 
 * @param q Quaternione originale.
 * @param q_conj Quaternione risultante coniugato.
 */
void quaternion_conjugate(const Quaternion &q, Quaternion &q_conj);

/**
 * @brief Moltiplica due quaternioni.
 * 
 * @param q1 Primo quaternione.
 * @param q2 Secondo quaternione.
 * @param q_result Quaternione risultante dalla moltiplicazione.
 */
void quaternion_multiply(const Quaternion &q1, const Quaternion &q2, Quaternion &q_result);

/**
 * @brief Converte un asse e un angolo in un quaternione.
 * 
 * @param axis Asse di rotazione.
 * @param angle_deg Angolo in gradi.
 * @param q Quaternione risultante.
 */
void quaternion_from_axis_angle(float axis[3], float angle_deg, Quaternion &q);

/**
 * @brief Normalizza un quaternione per renderlo unitario.
 * 
 * @param q Quaternione da normalizzare.
 */
void quaternion_normalize(Quaternion &q);

/**
 * @brief Compone una serie di quaternioni in un unico quaternione risultante.
 * 
 * @param quaternions Array di quaternioni da comporre.
 * @param count Numero di quaternioni nell'array.
 * @param result Quaternione risultante dalla composizione.
 */
void quaternion_compose(const Quaternion *quaternions, size_t count, Quaternion &result);

/**
 * @brief Calcola l'errore tra un quaternione desiderato e quello attuale.
 * 
 * @param desired Quaternione desiderato.
 * @param actual Quaternione attuale.
 * @param error Quaternione risultante che rappresenta l'errore.
 */
void quaternion_error(const Quaternion &desired, const Quaternion &actual, Quaternion &error);

#endif // QUATERNIONS_H
