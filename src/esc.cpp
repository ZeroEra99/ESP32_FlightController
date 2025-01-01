#include "ESC.h"

ESC::ESC(int pin, int analog_min, int analog_max) : Actuator(pin, analog_min, analog_max) {

}

void ESC::setup() {
  // Setup dell'ESC
    Serial.print("ESC ");Serial.print(pin);Serial.print(" setup starting.\n");
    actuator.attach(pin, analog_min, analog_max);
    actuator.writeMicroseconds(analog_min);
    Serial.print("ESC ");Serial.print(pin);Serial.print(" setup complete.\n");
}

void ESC::write(int value) {
  // Scrittura dell'ESC
  actuator.writeMicroseconds(value);
}