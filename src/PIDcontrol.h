/**
 * @file PIDcontrol.h
 * @brief Dichiarazioni per la classe PIDcontrol.
 *
 * La classe implementa un controllore PID con supporto per offset dinamici
 * e limite sull'integrale.
 */

#ifndef PID_CONTROL_H
#define PID_CONTROL_H

/**
 * @class PIDcontrol
 * @brief Implementazione di un controllore PID.
 *
 * Questa classe fornisce un controllore PID con termini proporzionale (P),
 * integrale (I) e derivativo (D). Include il supporto per offset dinamici
 * e un limite configurabile sull'integrale.
 */
class PIDcontrol
{
private:
    double kp;          ///< Guadagno proporzionale.
    double ki;          ///< Guadagno integrale.
    double kd;          ///< Guadagno derivativo.
    double lastError;   ///< Errore registrato nell'iterazione precedente.
    double integral;    ///< Valore accumulato dell'integrale.
    double maxIntegral; ///< Valore massimo consentito per l'integrale.

public:
    /**
     * @brief Costruttore della classe PIDcontrol.
     *
     * @param kp Guadagno proporzionale.
     * @param ki Guadagno integrale.
     * @param kd Guadagno derivativo.
     * @param maxIntegral Limite massimo per il valore integrale.
     */
    PIDcontrol(double kp, double ki, double kd, double maxIntegral);

    /**
     * @brief Calcola il valore di controllo PID.
     *
     * @param error Errore corrente.
     * @param dt Intervallo di tempo dall'ultimo aggiornamento.
     * @param kp_offset Offset per il guadagno proporzionale.
     * @param ki_offset Offset per il guadagno integrale.
     * @param kd_offset Offset per il guadagno derivativo.
     * @return Valore di controllo calcolato.
     */
    double pid(double error, double dt, double kp_offset, double ki_offset, double kd_offset);
};

#endif // PID_CONTROL_H
