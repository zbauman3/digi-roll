#include "./State.h"

void State::loop() {
  if (this->_pendingStateUpdate) {
    this->isUpdateLoop = true;
    this->_pendingStateUpdate = false;

    this->_mode = this->_nextMode;

    this->isModeIdle = this->_mode == STATE_MODE_IDLE;
    this->isModeReset = this->_mode == STATE_MODE_RESET;
    this->isModeTest = this->_mode == STATE_MODE_TEST;
  } else {
    this->isUpdateLoop = false;
    if (this->isModeReset) {
      this->setModeIdle();
    }
  }
}

void State::setMode(uint8_t nextMode) {
  this->_nextMode = nextMode;
  this->_pendingStateUpdate = true;
}

void State::setModeIdle() { this->setMode(STATE_MODE_IDLE); }
void State::setModeReset() { this->setMode(STATE_MODE_RESET); }
void State::setModeTest() { this->setMode(STATE_MODE_TEST); }
