/**
 * @file PIDcontrol.cpp
 * @brief Implementazione della classe PIDcontrol.
 *
 * Questa classe implementa un controllore PID con supporto per offset dinamici
 * sui guadagni e un limite configurabile sul termine integrale.
 */

#include "PIDcontrol.h"

/**
 * @brief Costruttore della classe PIDcontrol.
 *
 * Inizializza i parametri del controllore PID e i valori interni, come
 * l'integrale accumulato e l'ultimo errore registrato.
 *
 * @param kp Guadagno proporzionale.
 * @param ki Guadagno integrale.
 * @param kd Guadagno derivativo.
 * @param maxIntegral Limite massimo per il valore integrale.
 */
PIDcontrol::PIDcontrol(double kp, double ki, double kd, double maxIntegral) : kp(kp), ki(ki), kd(kd), maxIntegral(maxIntegral)
{
    integral = 0;  ///< Inizializza il valore integrale accumulato.
    lastError = 0; ///< Inizializza l'ultimo errore registrato.
}

/**
 * @brief Calcola il valore di controllo PID.
 *
 * Questo metodo utilizza l'errore corrente, il tempo trascorso dall'ultimo aggiornamento,
 * e gli offset sui guadagni per calcolare il valore di controllo PID. Il termine integrale
 * è limitato a un valore massimo configurabile per evitare il problema del windup.
 *
 * @param error Errore corrente rispetto al setpoint desiderato.
 * @param dt Intervallo di tempo trascorso dall'ultimo aggiornamento (in secondi).
 * @param kp_offset Offset dinamico per il guadagno proporzionale.
 * @param ki_offset Offset dinamico per il guadagno integrale.
 * @param kd_offset Offset dinamico per il guadagno derivativo.
 * @return Valore di controllo calcolato.
 */
double PIDcontrol::pid(double error, double dt, double kp_offset, double ki_offset, double kd_offset)
{
    // Calcolo del termine integrale con limitazione
    integral += error * dt;
    if (integral > maxIntegral)
        integral = maxIntegral;
    else if (integral < -maxIntegral)
        integral = -maxIntegral;

    // Calcolo del termine derivativo
    double derivative = (error - lastError) / dt;
    lastError = error;

    // Calcolo del valore di controllo PID
    return (kp + kp_offset) * error +
           (ki + ki_offset) * integral +
           (kd + kd_offset) * derivative;
}
