#include "Receiver.h"

PWM analog_input[IA6B_CHANNELS];

Receiver::Receiver(int *pins) {
  // Costruttore
  for (int i = 0; i < IA6B_CHANNELS; i++) {
    analog_input[i].attach(pins[i]);
    analog_input[i].begin(true);
  }
}


PilotData Receiver::read() {
  // Leggi i valori dal ricevitore
  PilotData pilot_data;
  for (int i = 0; i < IA6B_CHANNELS; i++) {
    if(pilot_data.data[i] = analog_input[i].getValue()==0)pilot_data.data[i]=NAN;
  }
  return pilot_data;
}
