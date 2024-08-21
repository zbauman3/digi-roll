#ifndef Z_LED_STRIP
#define Z_LED_STRIP

#include "SNX4HC595.h"
#include "models/State.h"
#include <AceRoutine.h>
#include <Arduino.h>

using namespace ace_routine;

class LedStrip : public Coroutine {
private:
  SNX4HC595Config config;
  State *state;

public:
  LedStrip(State *_state);
  void begin();
  void loop();
  int runCoroutine() override;
};

#endif // Z_LED_STRIP