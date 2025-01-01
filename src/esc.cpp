#include "ESC.h"

ESC::ESC(int pin, int pwm_min, int pwm_max) : Actuator(pin)
{
  Serial.print("ESC ");
  Serial.print(pin);
  Serial.print(" setup starting.\n");
  pwm_min = pwm_min;
  pwm_max = pwm_max;
  esc.attach(pin);
  esc.writeMicroseconds(pwm_min);
  Serial.print("ESC ");
  Serial.print(pin);
  Serial.print(" setup complete.\n");
}

void ESC::write(int value)
{
  // Scrittura dell'ESC
  esc.writeMicroseconds(value);
}