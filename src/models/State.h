#ifndef Z_STATE_H
#define Z_STATE_H

#include <Arduino.h>

#define STATE_MODE_IDLE 0
#define STATE_MODE_RESET 1
#define STATE_MODE_TEST 2
#define STATE_MODE_SELECT_DICE 3
#define STATE_BUTTON_NONE 255

class State {
private:
  volatile bool _pendingStateUpdate = false;
  volatile uint8_t _nextMode = STATE_MODE_IDLE;
  volatile uint8_t _nextButton = STATE_BUTTON_NONE;

  void setMode(uint8_t nextMode);

public:
  uint8_t mode = STATE_MODE_IDLE;
  uint8_t button = STATE_BUTTON_NONE;
  bool isUpdateLoop = false;
  bool isModeIdle = true;
  bool isModeReset = false;
  bool isModeTest = false;
  bool isModeSelectDice = false;

  void loop();

  // ---- setters
  void setModeIdle();
  void setModeReset();
  void setModeTest();
  void setModeSelectDice(uint8_t nextButton);
};

#endif // Z_STATE_H