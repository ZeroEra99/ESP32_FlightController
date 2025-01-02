/**
 * @file PIDcontrol.cpp
 * @brief Implementazione della classe PIDcontrol.
 */

#include "PIDcontrol.h"

/**
 * @brief Costruttore della classe PIDcontrol.
 * 
 * Inizializza i parametri del controllore PID e i valori interni come
 * l'integrale e l'ultimo errore.
 * 
 * @param kp Guadagno proporzionale.
 * @param ki Guadagno integrale.
 * @param kd Guadagno derivativo.
 * @param maxIntegral Limite massimo per il valore integrale.
 */
PIDcontrol::PIDcontrol(double kp, double ki, double kd, double maxIntegral) : kp(kp), ki(ki), kd(kd), maxIntegral(maxIntegral)
{
    integral = 0;
    lastError = 0;
}

/**
 * @brief Calcola il valore di controllo PID.
 * 
 * Esegue il calcolo PID considerando l'errore corrente, il tempo trascorso
 * dall'ultimo aggiornamento, e gli offset per i guadagni P, I e D.
 * L'integrale è limitato al valore massimo specificato per evitare accumuli eccessivi.
 * 
 * @param error Errore corrente.
 * @param dt Intervallo di tempo dall'ultimo aggiornamento.
 * @param kp_offset Offset per il guadagno proporzionale.
 * @param ki_offset Offset per il guadagno integrale.
 * @param kd_offset Offset per il guadagno derivativo.
 * @return Valore di controllo calcolato.
 */
double PIDcontrol::pid(double error, double dt, double kp_offset, double ki_offset, double kd_offset)
{
    integral += error * dt;
    if (integral > maxIntegral)
        integral = maxIntegral;
    else if (integral < -maxIntegral)
        integral = -maxIntegral;

    double derivative = (error - lastError) / dt;
    lastError = error;

    return (kp + kp_offset) * error + (ki + ki_offset) * integral + (kd + kd_offset) * derivative;
}
