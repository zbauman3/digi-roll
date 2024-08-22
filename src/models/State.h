#ifndef Z_STATE_H
#define Z_STATE_H

#include "utils/sleep.h"
#include <Arduino.h>

#define STATE_MODE_RESET 1
#define STATE_MODE_SELECT_DICE 2
#define STATE_MODE_RESULTS 3
#define STATE_MODE_ROLLING 4
#define STATE_MODE_SLEEP 5
#define STATE_MODE_WAKE 6
#define STATE_BRIGHTNESS_2 6
#define STATE_BRIGHTNESS_1 1
#define STATE_BRIGHTNESS_DELAY 5000
#define STATE_IDLE_TIMEOUT (STATE_BRIGHTNESS_DELAY * 2)

typedef struct {
  uint8_t mode;
  uint8_t dice;
  uint8_t diceCount;
  uint8_t brightness;
  uint8_t results[9];
  uint8_t resultIndex;
} StateData;

class State {
private:
  bool _pendingStateUpdate = true; // start with a pending update
  unsigned long _lastInteractionAt = 0;
  unsigned long _lastRolledAt = 0;
  StateData _nextData = {
      .mode = STATE_MODE_RESET,
      .dice = 0,
      .diceCount = 1,
      .brightness = STATE_BRIGHTNESS_2,
      .results = {1, 1, 1, 1, 1, 1, 1, 1, 1},
      .resultIndex = 0,
  };

public:
  bool isUpdateLoop = true; // start in an update loop
  StateData data = {
      .mode = STATE_MODE_RESET,
      .dice = 0,
      .diceCount = 1,
      .brightness = STATE_BRIGHTNESS_2,
      .results = {1, 1, 1, 1, 1, 1, 1, 1, 1},
      .resultIndex = 0,
  };

  void loop();

  // ---- setters
  void setModeResults();
  void triggerButton(uint8_t buttonPress);
  void triggerRoll();

  // ---- utils
  uint8_t getDiceCount();
};

#endif // Z_STATE_H