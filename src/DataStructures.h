#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "CONFIG.h"

#define EULER_ANGLES_SIZE 3
#define ANGULAR_VELOCITIES_SIZE 3
#define QUATERNION_SIZE 4

/**
 * Esempiodi enum class
 */
enum class Mode
{
  MANUAL,
  SEMI_AUTOMATIC,
  AUTOMATIC
};

enum Axis
{
  X,
  Y,
  Z,
  W
};


enum PilotInput{
  THROTTLE,
  PITCH,
  ROLL,
  YAW
};

enum class Color
{
  RED,
  GREEN,
  BLUE
};

enum class LightState{
  ON,
  OFF,
  BLINK
};


struct FlightData
{
  float angles[EULER_ANGLES_SIZE];
  float angular_velocities[ANGULAR_VELOCITIES_SIZE];
  float quaternion[QUATERNION_SIZE];
};

struct PilotData
{
  float data[IA6B_CHANNELS];
};

#endif // DATASTRUCTURES_H
