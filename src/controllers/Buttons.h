#ifndef Z_BUTTONS_H
#define Z_BUTTONS_H

#include "models/State.h"
#include <Arduino.h>

#define BUTTONS_0_PIN 0
#define BUTTONS_1_PIN 1
#define BUTTONS_2_PIN 2
#define BUTTONS_INT_PIN 3
#define BUTTONS_INT_VECT PCINT0_vect
#define BUTTONS_GIMSK PCIE0
#define BUTTONS_PCMSK0 PCINT7

class Buttons {
private:
  State *state;

public:
  Buttons(State *_state);
  void begin();
  void handleInterrupt();
};

#endif // Z_BUTTONS_H