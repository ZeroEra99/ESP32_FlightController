#ifndef FLIGHT_CONTROLLER_H
#define FLIGHT_CONTROLLER_H

#include "Receiver.h"
#include "bno055.h"
#include "Motor.h"
#include "ESC.h"
#include "PIDcontrol.h"
#include "FlightControllerConfig.h"

class FlightController
{
private:
    void start();
    void stop();
    void fail_safe();
    FlightData read_imu();

    // Componenti fisiche
    ESC esc;
    Motor servos[3];
    Receiver receiver;
    BNO055 imu;
    
    

    // Componenti logiche
    PIDcontrol pid_attitude[3]{
        PIDcontrol(KP_ATTITUDE_ROLL, KD_ATTITUDE_ROLL, KI_ATTITUDE_ROLL, MAX_INTEGRAL_ATTITUDE),
        PIDcontrol(KP_ATTITUDE_PITCH, KD_ATTITUDE_PITCH, KI_ATTITUDE_PITCH, MAX_INTEGRAL_ATTITUDE),
        PIDcontrol(KP_ATTITUDE_YAW, KD_ATTITUDE_YAW, KI_ATTITUDE_YAW, MAX_INTEGRAL_ATTITUDE)};
    PIDcontrol pid_gyro[3]{
        PIDcontrol(KP_GYRO_ROLL, KD_GYRO_ROLL, KI_GYRO_ROLL, MAX_INTEGRAL_GYRO),
        PIDcontrol(KP_GYRO_PITCH, KD_GYRO_PITCH, KI_GYRO_PITCH, MAX_INTEGRAL_GYRO),
        PIDcontrol(KP_GYRO_YAW, KD_GYRO_YAW, KI_GYRO_YAW, MAX_INTEGRAL_GYRO)};
    ControllerData data;

    // Stati del controllore
    STATE state;
    MODE mode;

public:
    FlightController();
    void setup();
    void control_loop();

    void input();
    void compute_data(double dt);
    void control();
    void output();
};

#endif // FLIGHT_CONTROLLER_H