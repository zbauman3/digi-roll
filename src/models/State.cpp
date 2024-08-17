#include "./State.h"

void State::loop() {
  if (this->_pendingStateUpdate) {
    this->_pendingStateUpdate = false;
    this->isUpdateLoop = true;
    this->updateState();

    if (this->isModeReset) {
      this->resetState();
    }

    return;
  }

  this->isUpdateLoop = false;

  // after a reset loop, transition back to idle
  if (this->isModeReset) {
    this->setModeIdle();
  }
}

void State::updateState() {
  this->stateData.dice = this->_nextStateData.dice;
  this->stateData.mode = this->_nextStateData.mode;
  this->stateData.diceCount = this->_nextStateData.diceCount;

  this->isModeIdle = this->stateData.mode == STATE_MODE_IDLE;
  this->isModeReset = this->stateData.mode == STATE_MODE_RESET;
  this->isModeTest = this->stateData.mode == STATE_MODE_TEST;
  this->isModeSelectDice = this->stateData.mode == STATE_MODE_SELECT_DICE;
}

void State::resetState() {
  this->stateData.dice = STATE_DICE_NONE;
  this->stateData.diceCount = 0;

  this->_nextStateData.dice = STATE_DICE_NONE;
  this->_nextStateData.diceCount = 0;
}

void State::setMode(uint8_t nextMode) {
  this->_nextStateData.mode = nextMode;
  this->_pendingStateUpdate = true;
}

void State::setModeIdle() { this->setMode(STATE_MODE_IDLE); }
void State::setModeReset() { this->setMode(STATE_MODE_RESET); }
void State::setModeTest() { this->setMode(STATE_MODE_TEST); }
void State::setModeSelectDice(uint8_t nextDice) {
  this->_nextStateData.dice = nextDice;
  if (this->stateData.dice == nextDice && this->stateData.diceCount < 9) {
    this->_nextStateData.diceCount = this->stateData.diceCount + 1;
  } else {
    this->_nextStateData.diceCount = 1;
  }
  this->setMode(STATE_MODE_SELECT_DICE);
}

uint8_t State::getDiceCount() {
  switch (this->stateData.dice) {
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