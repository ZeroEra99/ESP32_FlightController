#include "PIDcontroller.h"
#include "Logger.h"
#include <Arduino.h>

PIDcontroller::PIDcontroller(double kp, double ki, double kd, double maxIntegral) : kp(kp), ki(ki), kd(kd), maxIntegral(maxIntegral)
{
    // Inizializza i parametri del PID
    integral = 0;  ///< Inizializza il valore integrale accumulato.
    lastError = 0; ///< Inizializza l'ultimo errore registrato.
    Logger::getInstance().log(LogLevel::INFO, "PID controller initialized.");
}

double PIDcontroller::pid(double error, double dt, double kp_offset, double ki_offset, double kd_offset)
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
