#ifndef PID_CONTROL_H
#define PID_CONTROL_H

class PIDcontrol
{
private:
    double kp, ki, kd;
    double lastError, integral, maxIntegral;

public:
    PIDcontrol(double kp, double ki, double kd, double maxIntegral);
    double pid(double error, double dt);
};

#endif // PID_CONTROL_H