/**
 * @file IMU.h
 * @brief Dichiarazione della classe IMU per la gestione del sensore BNO055.
 */


#ifndef IMU_H
#define IMU_H

#include "DataStructures.h"

/**
 * @brief Classe per la gestione del sensore IMU (BNO055).
 *
 * Questa classe si occupa della configurazione e lettura dei dati dall'IMU.
 */
class IMU
{
public:
  /**
   * @brief Costruttore della classe IMU.
   *
   * Inizializza e configura il sensore BNO055.
   */
  IMU();

  /**
   * @brief Legge i dati dall'IMU.
   *
   * @param data Struttura che riceve i dati letti dall'IMU.
   * @return true Se la lettura ha avuto successo.
   * @return false Se si è verificato un errore durante la lettura.
   */
  bool read(ImuData &data);
};

#endif // IMU_H
