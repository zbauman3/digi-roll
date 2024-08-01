#ifndef Z_LED_STRIP
#define Z_LED_STRIP

#include "SNX4HC595.h"
#include "util/CoroutineAction.h"
#include <Arduino.h>

#define LED_STRIP_ACTION_WIPE 2

class LedStrip : public CoroutineAction {
private:
  SNX4HC595Config config;
  uint8_t i;

public:
  LedStrip();
  void begin();
  void wipe();
  void clear();
  int runCoroutine() override;
};

#endif // Z_LED_STRIP