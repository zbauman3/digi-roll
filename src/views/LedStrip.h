#ifndef Z_LED_STRIP
#define Z_LED_STRIP

#include "models/State.h"
#include "utils/Pins.h"
#include <AceRoutine.h>
#include <Arduino.h>

using namespace ace_routine;

class LedStrip : public Coroutine {
private:
  State *state;
  uint8_t sclk;
  uint8_t rclk;
  uint8_t data;
  void sendByte(uint8_t value);
  uint8_t diceToLedBitmap();

public:
  LedStrip(State *_state);
  void begin();
  void loop();
  int runCoroutine() override;
};

#endif // Z_LED_STRIP