#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "CONFIG.h"

enum Axis
{
  X,
  Y,
  Z,
  W
};

enum FlightDataInput
{
  EULER_ANGLES,
  ANGULAR_VELOCITIES,
  QUATERNION
};

enum PilotInput
{
  ROLL,
  PITCH,
  THROTTLE,
  YAW,
  AUX1,
  AUX2
};

struct FlightData
{
  float velocity[EULER_DIM];
  float acceleration[EULER_DIM];
  float angles[EULER_DIM];
  float angular_velocities[EULER_DIM];
  float quaternion[QUATERNION_DIM];
};

struct ControllerData
{
  float acceleration[EULER_DIM];
  float velocity[EULER_DIM];
  float error_angular_velocity[EULER_DIM];
  float error_angle[EULER_DIM];
  float error_quaternion[QUATERNION_DIM];
  float pid_tuning_offset[EULER_DIM];
  float pid_output[EULER_DIM];
};

struct PilotData
{
  float data[IA6B_CHANNELS];
};

enum PID
{
  KP,
  KI,
  KD
};

enum class Color
{
  RED,
  GREEN,
  BLUE
};

enum class LightState
{
  ON,
  OFF,
  BLINK
};

enum class STATE
{
  FAILSAFE = -1,
  DISARMED = 0,
  ARMED = 1
};

enum class MODE
{
  MANUAL = 0,
  GYRO_STABILIZED = 1
};


#endif // DATASTRUCTURES_H
