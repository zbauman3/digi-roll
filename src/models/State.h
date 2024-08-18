#ifndef Z_STATE_H
#define Z_STATE_H

#include <Arduino.h>

#define STATE_MODE_IDLE 0
#define STATE_MODE_RESET 1
#define STATE_MODE_SELECT_DICE 2
#define STATE_MODE_RESULTS 3
#define STATE_MODE_ROLLING 4
#define STATE_BRIGHTNESS_2 6
#define STATE_BRIGHTNESS_1 1
#define STATE_BRIGHTNESS_DELAY 3000
#define STATE_IDLE_TIMEOUT (STATE_BRIGHTNESS_DELAY * 2)

typedef struct {
  uint8_t mode;
  uint8_t dice;
  uint8_t diceCount;
  uint8_t brightness;
  long results[9];
  uint8_t resultIndex;
} StateData;

class State {
private:
  bool _pendingStateUpdate = false;
  unsigned long _lastInteractionAt = 0;
  unsigned long _lastRolledAt = 0;
  StateData _nextData = {
      .mode = STATE_MODE_IDLE,
      .dice = 0,
      .diceCount = 0,
      .brightness = STATE_BRIGHTNESS_2,
      .results = {1, 1, 1, 1, 1, 1, 1, 1, 1},
      .resultIndex = 0,
  };

public:
  bool isUpdateLoop = false;
  StateData data = {
      .mode = STATE_MODE_IDLE,
      .dice = 0,
      .diceCount = 0,
      .brightness = STATE_BRIGHTNESS_2,
      .results = {1, 1, 1, 1, 1, 1, 1, 1, 1},
      .resultIndex = 0,
  };
  bool isModeIdle = true;
  bool isModeReset = false;
  bool isModeSelectDice = false;
  bool isModeRolling = false;
  bool isModeResults = false;

  void loop();

  // ---- setters
  void setModeReset();
  void setModeResults();

  void setBrightness(uint8_t brightness);
  void triggerButton(uint8_t buttonPress);
  void triggerRoll();

  // -- utils
  uint8_t getDiceCount();
};

#endif // Z_STATE_H