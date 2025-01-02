#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "CONFIG.h"
#include "HardwareParameters.h"

/* ========================
 *   COSTANTI
 * ======================== */
#define QUATERNION_DIM 4
#define EULER_DIM 3

/* ========================
 *   ENUMERAZIONI
 * ======================== */
enum Axis
{
  W,
  X,
  Y,
  Z
};

enum PilotInput
{
  ROLL,
  PITCH,
  THROTTLE,
  YAW,
  SWA,
  SWB,
  SWC,
  SWD,
  VRA,
  VRB
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

enum class CONTROLLER_MODE
{
  STANDARD = 0,
  KP_CALIBRATION,
  KI_CALIBRATION,
  KD_CALIBRATION
};

enum class ASSIST_MODE
{
  MANUAL = 0,
  GYRO_STABILIZED = 1,
  ATTITUDE_CONTROL = 2,
};

/* ========================
 *   STRUTTURE DATI
 * ======================== */
struct FlightData
{
  float velocity[EULER_DIM];
  float acceleration[EULER_DIM];
  float gyro[EULER_DIM];
  float attitude[QUATERNION_DIM];
  float position[EULER_DIM];
};

struct ControllerData
{
  float user_input[IA6B_CHANNELS];
  float setpoint_attitude[QUATERNION_DIM];

  float acceleration[EULER_DIM];
  float velocity[EULER_DIM];
  float gyro[EULER_DIM];
  float attitude[QUATERNION_DIM];

  float error_gyro[EULER_DIM];
  float error_attitude[QUATERNION_DIM];
  float pid_tuning_offset[EULER_DIM];
  float pid_output[EULER_DIM];
};

#endif // DATASTRUCTURES_H
