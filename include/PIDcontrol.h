/**
 * @file PIDcontrol.h
 * @brief Dichiarazione della classe PIDcontrol per la gestione del controllo PID.
 * 
 * Questa classe implementa un controllore PID con supporto per limiti dell'integrale
 * e offset dinamici per il tuning in tempo reale.
 */

#ifndef PID_CONTROL_H
#define PID_CONTROL_H

/**
 * @brief Classe per il controllo PID.
 * 
 * Fornisce un metodo per calcolare il valore di controllo PID in base agli errori
 * e ai guadagni proporzionali, integrali e derivativi.
 */
class PIDcontrol
{
private:
    double kp;          ///< Guadagno proporzionale.
    double ki;          ///< Guadagno integrale.
    double kd;          ///< Guadagno derivativo.
    double maxIntegral; ///< Limite massimo per il valore integrale.

    double integral;    ///< Accumulatore per il valore integrale.
    double lastError;   ///< Ultimo errore registrato.

public:
    /**
     * @brief Costruttore della classe PIDcontrol.
     * 
     * @param kp Guadagno proporzionale iniziale.
     * @param ki Guadagno integrale iniziale.
     * @param kd Guadagno derivativo iniziale.
     * @param maxIntegral Limite massimo per il valore integrale.
     */
    PIDcontrol(double kp, double ki, double kd, double maxIntegral);

    /**
     * @brief Calcola il valore di controllo PID.
     * 
     * @param error Errore attuale.
     * @param dt Intervallo di tempo dall'ultimo calcolo.
     * @param kp_offset Offset dinamico per il guadagno proporzionale.
     * @param ki_offset Offset dinamico per il guadagno integrale.
     * @param kd_offset Offset dinamico per il guadagno derivativo.
     * @return Valore di controllo calcolato.
     */
    double pid(double error, double dt, double kp_offset, double ki_offset, double kd_offset);
};

#endif // PID_CONTROL_H
