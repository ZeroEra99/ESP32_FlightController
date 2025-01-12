#include "PIDcontrol.h"
//#include "DebugLogger.h"
#include <Arduino.h>

PIDcontrol::PIDcontrol(double kp, double ki, double kd, double maxIntegral) : kp(kp), ki(ki), kd(kd), maxIntegral(maxIntegral)
{
    // Inizializza i parametri del PID
    integral = 0;  ///< Inizializza il valore integrale accumulato.
    lastError = 0; ///< Inizializza l'ultimo errore registrato.
    //DebugLogger::getInstance()->log("PID controller initialized.", LogLevel::DEBUG);
    Serial.println("PID controller initialized.");
}

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
