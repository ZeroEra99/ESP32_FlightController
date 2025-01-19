/**
 * @file SystemController.h
 * @brief Dichiarazione della classe SystemController per la gestione dello stato del sistema.
 */

#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include "DataStructures.h"

/**
 * @brief Classe per la gestione del sistema di controllo.
 *
 * Si occupa di monitorare lo stato del controller, verificare le condizioni di armamento/disarmamento
 * e aggiornare le modalità operative in base agli input del pilota.
 */
class SystemController
{
private:
    /**
     * @brief Avvia il sistema, portandolo in stato armato.
     */
    void start();

    /**
     * @brief Ferma il sistema, portandolo in stato disarmato.
     */
    void stop();

    /**
     * @brief Attiva la modalità di assistenza selezionata.     *
     */
    void failSafe();

    /**
     * @brief Verifica se sono soddisfatte le condizioni per disarmare il sistema.
     *
     * @param receiver_data Dati ricevuti dal pilota.
     * @return true Se le condizioni di disarmo sono soddisfatte.
     * @return false Altrimenti.
     */
    bool check_disarm_conditions(ReceiverData &receiver_data);

    /**
     * @brief Verifica se sono soddisfatte le condizioni per armare il sistema.
     *
     * @param receiver_data Dati ricevuti dal pilota.
     * @return true Se le condizioni di armamento sono soddisfatte.
     * @return false Altrimenti.
     */
    bool check_arm_conditions(ReceiverData &receiver_data);

public:
    /**
     * @brief Costruttore della classe SystemController.
     *
     * Inizializza lo stato del controller e i parametri di configurazione.
     */
    SystemController();

    /**
     * @brief Verifica e gestisce eventuali errori nel sistema.
     */
    void check_errors();

    /**
     * @brief Avvia o ferma il sistema in base alle condizioni di armamento/disarmamento.
     */
    void update_state(ReceiverData &receiver_data);

    /**
     * @brief Aggiorna le modalità operative del controller.
     *
     * Aggiorna lo stato del sistema in base agli input ricevuti dal pilota.
     *
     * @param receiver_data Dati ricevuti dal pilota.
     */
    void update_modes(ReceiverData &receiver_data);

    /**
     * @brief Imposta i valori di output per gli attuatori.
     *
     * Aggiorna l'output in base allo stato e alla modalità operativa.
     *
     * @param output Riferimento alla struttura di output da aggiornare.
     * @param receiver_data Dati ricevuti dal pilota.
     */
    void set_output(Output output, ReceiverData &receiver_data);

    // Stati del controllore
    CONTROLLER_STATE state;                ///< Stato attuale del controller.
    ASSIST_MODE assist_mode;               ///< Modalità di assistenza corrente.
    CONTROLLER_MODE controller_mode;       ///< Modalità di controllo corrente.
    CALIBRATION_TARGET calibration_target; ///< Asse target per la calibrazione PID.
    Errors error;                          ///< Errori del sistema.
};

#endif // SYSTEM_CONTROLLER_H
