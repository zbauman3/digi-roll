#ifndef Z_STATE_H
#define Z_STATE_H

#include <Arduino.h>

#define STATE_MODE_IDLE 0
#define STATE_MODE_RESET 1
#define STATE_MODE_TEST 2

class State {
private:
  bool _pendingStateUpdate = false;
  uint8_t _nextMode = STATE_MODE_IDLE;
  uint8_t _mode = STATE_MODE_IDLE;

  void setMode(uint8_t nextMode);

public:
  bool isUpdateLoop;
  bool isModeIdle;
  bool isModeReset;
  bool isModeTest;

  void loop();

  // ---- mode setters
  void setModeIdle();
  void setModeReset();
  void setModeTest();
};

#endif // Z_STATE_H