#include "./State.h"

void State::loop() {
  unsigned long idleTime = millis() - this->_lastInteractionAt;
  if (idleTime > STATE_BRIGHTNESS_DELAY) {
    if (!this->isModeResults && idleTime > STATE_IDLE_TIMEOUT) {
      this->setModeReset();
    } else {
      this->setBrightness(STATE_BRIGHTNESS_1);
    }
  } else if (this->data.brightness != STATE_BRIGHTNESS_2) {
    this->setBrightness(STATE_BRIGHTNESS_2);
  }

  if (this->_pendingStateUpdate) {
    this->_pendingStateUpdate = false;
    this->isUpdateLoop = true;

    this->data.mode = this->_nextData.mode;
    this->data.dice = this->_nextData.dice;
    this->data.diceCount = this->_nextData.diceCount;
    this->data.brightness = this->_nextData.brightness;

    this->data.results[0] = this->_nextData.results[0];
    this->data.results[1] = this->_nextData.results[1];
    this->data.results[2] = this->_nextData.results[2];
    this->data.results[3] = this->_nextData.results[3];
    this->data.results[4] = this->_nextData.results[4];
    this->data.results[5] = this->_nextData.results[5];
    this->data.results[6] = this->_nextData.results[6];
    this->data.results[7] = this->_nextData.results[7];
    this->data.results[8] = this->_nextData.results[8];
    this->data.resultIndex = this->_nextData.resultIndex;

    this->isModeIdle = this->data.mode == STATE_MODE_IDLE;
    this->isModeReset = this->data.mode == STATE_MODE_RESET;
    this->isModeSelectDice = this->data.mode == STATE_MODE_SELECT_DICE;
    this->isModeResults = this->data.mode == STATE_MODE_RESULTS;
    this->isModeRolling = this->data.mode == STATE_MODE_ROLLING;

    if (this->isModeReset) {
      this->data.dice = 0;
      this->data.diceCount = 0;
      this->data.brightness = STATE_BRIGHTNESS_2;

      this->data.results[0] = this->_nextData.results[0] = 0;
      this->data.results[1] = this->_nextData.results[1] = 0;
      this->data.results[2] = this->_nextData.results[2] = 0;
      this->data.results[3] = this->_nextData.results[3] = 0;
      this->data.results[4] = this->_nextData.results[4] = 0;
      this->data.results[5] = this->_nextData.results[5] = 0;
      this->data.results[6] = this->_nextData.results[6] = 0;
      this->data.results[7] = this->_nextData.results[7] = 0;
      this->data.results[8] = this->_nextData.results[8] = 0;
      this->data.resultIndex = 0;

      this->_nextData.dice = 0;
      this->_nextData.diceCount = 0;
      this->_nextData.brightness = STATE_BRIGHTNESS_2;
      this->_nextData.resultIndex = 0;
    }

    return;
  }

  this->isUpdateLoop = false;

  // after a reset loop, transition back to idle
  if (this->isModeReset) {
    this->_nextData.mode = STATE_MODE_IDLE;
    this->_pendingStateUpdate = true;
  }
}

void State::setModeReset() {
  this->_lastInteractionAt = millis();
  this->_nextData.mode = STATE_MODE_RESET;
  this->_pendingStateUpdate = true;
}

void State::setModeResults() {
  this->_nextData.mode = STATE_MODE_RESULTS;
  this->_pendingStateUpdate = true;
}

void State::setBrightness(uint8_t brightness) {
  if (this->data.brightness != brightness) {
    this->_nextData.brightness = brightness;
    this->_pendingStateUpdate = true;
  }
}

void State::triggerButton(uint8_t buttonPress) {
  if (buttonPress == 7) {
    this->setModeReset();
    return;
  }

  this->_lastInteractionAt = millis();

  if (this->isModeResults) {
    if (this->data.dice != buttonPress) {
      return;
    }

    if (this->data.resultIndex + 2 > this->data.diceCount) {
      this->_nextData.resultIndex = 0;
    } else {
      this->_nextData.resultIndex = this->data.resultIndex + 1;
    }

    this->_pendingStateUpdate = true;
  } else if (!this->isModeRolling) {
    this->_nextData.dice = buttonPress;
    if (this->data.dice == buttonPress && this->data.diceCount < 9) {
      this->_nextData.diceCount = this->data.diceCount + 1;
    } else {
      this->_nextData.diceCount = 1;
    }

    this->_nextData.mode = STATE_MODE_SELECT_DICE;
    this->_pendingStateUpdate = true;
  }
}

void State::triggerRoll() {
  if (!this->isModeSelectDice) {
    return;
  }

  if (this->_lastRolledAt == 0) {
    this->_lastRolledAt = millis();
    this->_lastInteractionAt = this->_lastRolledAt;
    return;
  }

  unsigned long now = millis();
  if (now - this->_lastRolledAt < 150) {
    return;
  }

  this->_lastInteractionAt = now;
  unsigned long seedBase = now - this->_lastRolledAt;
  this->_lastRolledAt = 0;

  uint8_t diceCountMax = this->getDiceCount() + 1;
  for (uint8_t i = 0; i < this->data.diceCount; i++) {
    randomSeed(seedBase + i);
    this->_nextData.results[i] = random(1, diceCountMax);
    delay(((uint8_t)(seedBase % this->data.diceCount)) + 1 + i);
  }

  this->_nextData.mode = STATE_MODE_ROLLING;
  this->_pendingStateUpdate = true;
}

uint8_t State::getDiceCount() {
  switch (this->data.dice) {
  case 0:
    return 4;
  case 1:
    return 6;
  case 2:
    return 8;
  case 3:
    return 10;
  case 4:
    return 12;
  case 6:
    return 100;
  case 5:
  default:
    return 20;
  }
}