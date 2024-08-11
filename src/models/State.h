#ifndef Z_STATE_H
#define Z_STATE_H

#include <Arduino.h>

#define STATE_MODE_IDLE 0
#define STATE_MODE_TEST 1

class State {
private:
  bool pendingStateUpdate = true;
  uint8_t nextMode = STATE_MODE_IDLE;

  bool _isUpdateLoop = true;
  uint8_t _mode = STATE_MODE_IDLE;

public:
  bool isModeIdle;
  bool isModeTest;

  bool isMode(uint8_t mode);
  bool isUpdateLoop();
  void loop();

  // ---- mode setters
  void setModeTest();
};

#endif // Z_STATE_H