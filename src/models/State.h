#ifndef Z_STATE_H
#define Z_STATE_H

#include <Arduino.h>

#define STATE_MODE_IDLE 0
#define STATE_MODE_RESET 1
#define STATE_MODE_TEST 2
#define STATE_MODE_SELECT_DICE 3
#define STATE_MODE_RESULTS 4
#define STATE_DICE_NONE 255
#define STATE_BRIGHTNESS_2 4
#define STATE_BRIGHTNESS_1 1
#define STATE_BRIGHTNESS_DELAY 3500
#define STATE_IDLE_TIMEOUT (STATE_BRIGHTNESS_DELAY * 2)

typedef struct {
  uint8_t mode;
  uint8_t dice;
  uint8_t diceCount;
  uint8_t brightness;
} StateData;

class State {
private:
  bool _pendingStateUpdate = false;
  unsigned long _lastInteractionAt = 0;
  StateData _nextData = {
      .mode = STATE_MODE_IDLE,
      .dice = STATE_DICE_NONE,
      .diceCount = 0,
      .brightness = STATE_BRIGHTNESS_2,
  };
  void setMode(uint8_t nextMode);
  void update();
  void reset();

public:
  bool isUpdateLoop = false;
  StateData data = {
      .mode = STATE_MODE_IDLE,
      .dice = STATE_DICE_NONE,
      .diceCount = 0,
      .brightness = STATE_BRIGHTNESS_2,
  };
  bool isModeIdle = true;
  bool isModeReset = false;
  bool isModeTest = false;
  bool isModeSelectDice = false;
  bool isModeResults = false;

  void loop();

  // ---- setters
  void setModeIdle();
  void setModeReset();
  void setModeTest();
  void setModeSelectDice(uint8_t nextDice);

  void setBrightness(uint8_t nextDice);

  // -- utils
  uint8_t getDiceCount();
};

#endif // Z_STATE_H