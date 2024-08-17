#ifndef Z_STATE_H
#define Z_STATE_H

#include <Arduino.h>

#define STATE_MODE_IDLE 0
#define STATE_MODE_RESET 1
#define STATE_MODE_TEST 2
#define STATE_MODE_SELECT_DICE 3
#define STATE_DICE_NONE 255

typedef struct {
  uint8_t mode;
  uint8_t dice;
  uint8_t diceCount;
} StateData;

class State {
private:
  bool _pendingStateUpdate = false;
  StateData _nextData = {
      .dice = STATE_DICE_NONE,
      .mode = STATE_MODE_IDLE,
      .diceCount = 0,
  };
  void setMode(uint8_t nextMode);
  void update();
  void reset();

public:
  bool isUpdateLoop = false;
  StateData data = {
      .dice = STATE_DICE_NONE,
      .mode = STATE_MODE_IDLE,
      .diceCount = 0,
  };
  bool isModeIdle = true;
  bool isModeReset = false;
  bool isModeTest = false;
  bool isModeSelectDice = false;

  void loop();

  // ---- setters
  void setModeIdle();
  void setModeReset();
  void setModeTest();
  void setModeSelectDice(uint8_t nextDice);

  // -- utils
  uint8_t getDiceCount();
};

#endif // Z_STATE_H