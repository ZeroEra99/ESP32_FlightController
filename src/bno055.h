/**
 * @file BNO055.h
 * @brief Dichiarazioni per la classe BNO055.
 * 
 * La classe BNO055 fornisce un'interfaccia per il sensore di orientamento BNO055,
 * che include accelerometro, giroscopio e magnetometro.
 */

#ifndef BNO_055_H
#define BNO_055_H

#include "DataStructures.h"

/**
 * @class BNO055
 * @brief Interfaccia per il sensore di orientamento BNO055.
 * 
 * Questa classe consente di configurare il sensore BNO055 e di leggere i dati di orientamento,
 * velocità angolari e accelerazioni.
 */
class BNO055 {
private:
    /**
     * @brief Configura il sensore BNO055.
     * 
     * Metodo privato utilizzato per inizializzare e configurare il sensore al momento
     * della creazione dell'oggetto.
     */
    void setup();

public:
    /**
     * @brief Costruttore della classe BNO055.
     * 
     * Inizializza il sensore e lo configura utilizzando il metodo `setup`.
     */
    BNO055();

    /**
     * @brief Legge i dati dal sensore.
     * 
     * Recupera i dati di velocità angolare, accelerazione e orientamento
     * dal sensore BNO055 e li restituisce in una struttura di tipo `FlightData`.
     * 
     * @return Struttura `FlightData` contenente i dati letti dal sensore.
     */
    FlightData read();
};

#endif  // BNO_055_H
