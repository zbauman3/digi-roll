#include "./State.h"

void State::loop() {
  if (this->_pendingStateUpdate) {
    this->isUpdateLoop = true;
    this->_pendingStateUpdate = false;

    this->mode = this->_nextMode;
    this->button = this->_nextButton;

    this->isModeIdle = this->mode == STATE_MODE_IDLE;
    this->isModeReset = this->mode == STATE_MODE_RESET;
    this->isModeTest = this->mode == STATE_MODE_TEST;
    this->isModeSelectDice = this->mode == STATE_MODE_SELECT_DICE;

    if (this->isModeReset) {
      this->button = STATE_BUTTON_NONE;
    }
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
void State::setModeSelectDice(uint8_t nextButton) {
  this->_nextButton = nextButton;
  this->setMode(STATE_MODE_SELECT_DICE);
}