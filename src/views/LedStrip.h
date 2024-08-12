#ifndef Z_LED_STRIP
#define Z_LED_STRIP

#include "SNX4HC595.h"
#include "models/State.h"
#include "util/CoroutineAction.h"
#include <Arduino.h>

#define LED_STRIP_ACTION_WIPE 2
#define LED_STRIP_ACTION_BYTE 3

class LedStrip : public CoroutineAction {
private:
  SNX4HC595Config config;
  uint8_t i;
  uint8_t disp;
  State *state;

public:
  LedStrip(State *_state);
  void begin();
  void clear();
  void loop() override;
  int runCoroutine() override;

  // -------- helpers
  void wipe();
  void showByte(uint8_t _disp);
};

#endif // Z_LED_STRIP