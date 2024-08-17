#include "./State.h"

void State::loop() {
  if (this->_pendingStateUpdate) {
    this->_pendingStateUpdate = false;
    this->isUpdateLoop = true;
    this->update();

    if (this->isModeReset) {
      this->reset();
    }

    return;
  }

  this->isUpdateLoop = false;

  // after a reset loop, transition back to idle
  if (this->isModeReset) {
    this->setModeIdle();
  }
}

void State::update() {
  this->data.dice = this->_nextData.dice;
  this->data.mode = this->_nextData.mode;
  this->data.diceCount = this->_nextData.diceCount;

  this->isModeIdle = this->data.mode == STATE_MODE_IDLE;
  this->isModeReset = this->data.mode == STATE_MODE_RESET;
  this->isModeTest = this->data.mode == STATE_MODE_TEST;
  this->isModeSelectDice = this->data.mode == STATE_MODE_SELECT_DICE;
}

void State::reset() {
  this->data.dice = STATE_DICE_NONE;
  this->data.diceCount = 0;

  this->_nextData.dice = STATE_DICE_NONE;
  this->_nextData.diceCount = 0;
}

void State::setMode(uint8_t nextMode) {
  this->_nextData.mode = nextMode;
  this->_pendingStateUpdate = true;
}

void State::setModeIdle() { this->setMode(STATE_MODE_IDLE); }
void State::setModeReset() { this->setMode(STATE_MODE_RESET); }
void State::setModeTest() { this->setMode(STATE_MODE_TEST); }
void State::setModeSelectDice(uint8_t nextDice) {
  this->_nextData.dice = nextDice;
  if (this->data.dice == nextDice && this->data.diceCount < 9) {
    this->_nextData.diceCount = this->data.diceCount + 1;
  } else {
    this->_nextData.diceCount = 1;
  }
  this->setMode(STATE_MODE_SELECT_DICE);
}

uint8_t State::getDiceCount() {
  switch (this->data.dice) {
  case 0:
    return 4;
    break;
  case 1:
    return 6;
    break;
  case 2:
    return 8;
    break;
  case 3:
    return 10;
    break;
  case 4:
    return 12;
    break;
  case 6:
    return 100;
    break;
  case 5:
  default:
    return 20;
    break;
  }
}