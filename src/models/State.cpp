#include "./State.h"

bool State::isUpdateLoop() { return this->_isUpdateLoop; }

void State::loop() {
  if (this->pendingStateUpdate) {
    this->_isUpdateLoop = true;
    this->pendingStateUpdate = false;

    this->_mode = this->nextMode;

    this->isModeIdle = this->_mode == STATE_MODE_IDLE;
    this->isModeTest = this->_mode == STATE_MODE_TEST;
  } else {
    this->_isUpdateLoop = false;
  }
}

void State::setModeTest() {
  this->nextMode = STATE_MODE_TEST;
  this->pendingStateUpdate = true;
}
