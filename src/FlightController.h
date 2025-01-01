#ifndef FLIGHT_CONTROLLER_H
#define FLIGHT_CONTROLLER_H

#include "Aircraft.h"
#include "Receiver.h"
#include "PIDcontrol.h"

class FlightController
{
private:
    Aircraft aircraft;
    Receiver receiver;
    
    PIDcontrol pid_angle[3]{
        PIDcontrol(KP_ANGLE_ROLL, KD_ANGLE_ROLL, KI_ANGLE_ROLL, MAX_INTEGRAL_ANGLE),
        PIDcontrol(KP_ANGLE_PITCH, KD_ANGLE_PITCH, KI_ANGLE_PITCH, MAX_INTEGRAL_ANGLE),
        PIDcontrol(KP_ANGLE_YAW, KD_ANGLE_YAW, KI_ANGLE_YAW, MAX_INTEGRAL_ANGLE)};
    PIDcontrol pid_angular_velocity[3]{
        PIDcontrol(KP_ANGULAR_VELOCITY_ROLL, KD_ANGULAR_VELOCITY_ROLL, KI_ANGULAR_VELOCITY_ROLL, MAX_INTEGRAL_ANGULAR_VELOCITY),
        PIDcontrol(KP_ANGULAR_VELOCITY_PITCH, KD_ANGULAR_VELOCITY_PITCH, KI_ANGULAR_VELOCITY_PITCH, MAX_INTEGRAL_ANGULAR_VELOCITY),
        PIDcontrol(KP_ANGULAR_VELOCITY_YAW, KD_ANGULAR_VELOCITY_YAW, KI_ANGULAR_VELOCITY_YAW, MAX_INTEGRAL_ANGULAR_VELOCITY)};

    ControllerData data;
    STATE state;

public:
    FlightController();
    void setup();
    void control_loop();

    void start();
    void stop();
    void fail_safe();

    void get_data();
    void control();
};

#endif // FLIGHT_CONTROLLER_H