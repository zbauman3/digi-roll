#ifndef Z_HALL_EFFECT
#define Z_HALL_EFFECT

#include "models/State.h"
#include "utils/Pins.h"
#include <Arduino.h>

class HallEffect {
private:
  State *state;
  volatile bool didInterrupt = false;
  bool lastState = false;
  bool isEnabled = false;

public:
  HallEffect(State *_state);
  void begin();
  void loop();
  void handleInterrupt();
};

#endif // Z_HALL_EFFECT