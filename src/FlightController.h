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

    int pilot_input[IA6B_CHANNELS];

    FlightData flight_data;

    PIDcontrol angle[3]{PIDcontrol(1, 0, 0, 0), PIDcontrol(1, 0, 0, 0), PIDcontrol(1, 0, 0, 0)};
    PIDcontrol angular_velocity[3]{PIDcontrol(1, 0, 0, 0), PIDcontrol(1, 0, 0, 0), PIDcontrol(1, 0, 0, 0)};

public:
    FlightController();
    void setup();
    void control_loop();

    void get_pilot_data();
    void get_imu_data();
};



#endif // FLIGHT_CONTROLLER_H