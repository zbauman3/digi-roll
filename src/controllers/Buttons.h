#ifndef Z_BUTTONS_H
#define Z_BUTTONS_H

#include "models/State.h"
#include "util/atomic.h"
#include "utils/Pins.h"
#include <Arduino.h>

class Buttons {
private:
  State *state;
  volatile bool didInterrupt = false;
  volatile uint8_t lastPressed[3] = {0, 0, 0};
  unsigned long lastPressedAt = 0;

public:
  Buttons(State *_state);
  void begin();
  void loop();
  void handleInterrupt();
};

#endif // Z_BUTTONS_H