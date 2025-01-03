/**
 * @file BNO055.h
 * @brief Dichiarazioni per la classe BNO055.
 *
 * La classe BNO055 fornisce un'interfaccia per il sensore di orientamento BNO055,
 * che include accelerometro, giroscopio e magnetometro. Questa classe consente
 * di configurare il sensore e di leggere dati utili per il controllo di volo.
 */

#ifndef BNO_055_H
#define BNO_055_H

#include "DataStructures.h"

/**
 * @class BNO055
 * @brief Interfaccia per il sensore di orientamento BNO055.
 *
 * Questa classe consente di configurare e utilizzare il sensore BNO055 per ottenere:
 * - Orientamento sotto forma di quaternioni.
 * - Velocità angolari dal giroscopio.
 * - Accelerazioni lineari dall'accelerometro.
 *
 * I dati vengono restituiti in una struttura `FlightData` per un'integrazione
 * semplificata con altre componenti del sistema.
 */
class BNO055
{
private:
  /**
   * @brief Configura il sensore BNO055.
   *
   * Metodo privato utilizzato per inizializzare e configurare il sensore.
   * Deve essere invocato manualmente per abilitare il sensore prima dell'uso.
   */
  void setup();

public:
  /**
   * @brief Costruttore della classe BNO055.
   *
   * Inizializza l'oggetto associato al sensore. La configurazione del sensore
   * deve essere completata chiamando il metodo `setup`.
   */
  BNO055();

  /**
   * @brief Legge i dati dal sensore.
   *
   * Questo metodo raccoglie i dati di:
   * - Velocità angolari (giroscopio).
   * - Orientamento (quaternioni).
   * - Accelerazioni lineari.
   *
   * Inoltre, integra l'accelerazione per calcolare la velocità lineare.
   *
   * @return Una struttura `FlightData` contenente:
   * - Velocità angolari (gyro).
   * - Orientamento (attitude).
   * - Accelerazione lineare.
   * - Velocità lineare calcolata.
   */
  FlightData read();
};

#endif // BNO_055_H
