#include "FlightController.h"
#include "Utils.h"

int receiver_pins[IA6B_CHANNELS] = {};

FlightController::FlightController() : aircraft(), receiver(Receiver(receiver_pins))
{
}

void FlightController::setup()
{
}

void FlightController::control_loop()
{
    Serial.print("Control loop starting.\n");
    while (true)
    {
    }
}

void FlightController::get_pilot_data()
{
    int *data = new int[IA6B_CHANNELS];
    data = receiver.read();

    for (int i = 0; i < IA6B_CHANNELS; i++)
    {
        if (data[i] == NAN)
            ; // failsafe
    }
    pilot_input[ROLL] = map(data[ROLL], 1000, 2000, -90, 90); // Macro da definire
    pilot_input[PITCH] = map(data[PITCH], 1000, 2000, -90, 90);
    pilot_input[THROTTLE] = map(data[THROTTLE], 1000, 2000, 0, 100);
    pilot_input[YAW] = map(data[YAW], 1000, 2000, -90, 90);
    pilot_input[AUX1] = map(data[AUX1], 1000, 2000, 0, 100);
    pilot_input[AUX2] = map(data[AUX2], 1000, 2000, 0, 100);
}

void FlightController::get_imu_data()
{
    flight_data = aircraft.read_imu();
}
