#ifndef PIDCONTROL_H
#define PIDCONTROL_H

class PIDcontrol
{
private:
    double kp, ki, kd;
    double lastError, integral, maxIntegral;

public:
    PIDcontrol(double kp, double ki, double kd, double maxIntegral);
    double pid(double error, double dt);
};

#endif // PIDCONTROL_H