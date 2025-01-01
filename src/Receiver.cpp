#include "Receiver.h"

PWM analog_input[IA6B_CHANNELS];

Receiver::Receiver(int *pins) {
  // Costruttore
  for (int i = 0; i < IA6B_CHANNELS; i++) {
    analog_input[i].attach(pins[i]);
    analog_input[i].begin(true);
  }
}


int* Receiver::read() {
  // Leggi i valori dal ricevitore
  int data[IA6B_CHANNELS];
  for (int i = 0; i < IA6B_CHANNELS; i++) {
    data[i] = analog_input[i].getValue();
  }
  return data;
}
