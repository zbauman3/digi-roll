#include "./State.h"

void State::loop() {
  unsigned long idleTime = millis() - this->_lastInteractionAt;
  // check if the user has been idle for some time
  if (idleTime > STATE_BRIGHTNESS_DELAY) {
    if (idleTime > STATE_IDLE_TIMEOUT) {
      // if idle past the timeout and in dice select, or
      // on results and past results timeout, trigger sleep
      if (this->data.mode == STATE_MODE_SELECT_DICE ||
          (this->data.mode == STATE_MODE_RESULTS &&
           idleTime > STATE_RESULTS_TIMEOUT)) {
        this->_nextData.mode = STATE_MODE_RESET;
        this->_pendingStateUpdate = true;
      }
      // otherwise, dim the brightness
    } else if (this->data.brightness != STATE_BRIGHTNESS_1 &&
               this->data.mode != STATE_MODE_RESET &&
               this->_nextData.mode != STATE_MODE_RESET) {
      this->_nextData.brightness = STATE_BRIGHTNESS_1;
      this->_pendingStateUpdate = true;
    }
    // otherwise, reset the brightness
  } else if (this->data.brightness != STATE_BRIGHTNESS_2) {
    this->_nextData.brightness = STATE_BRIGHTNESS_2;
    this->_pendingStateUpdate = true;
  }

  // if an update is scheduled, apply the update
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

    // if this was a transition to a reset mode, clear all data
    if (this->data.mode == STATE_MODE_RESET) {
      this->data.dice = 0;
      this->data.diceCount = 1;
      this->data.brightness = STATE_BRIGHTNESS_2;
      this->_lastRolledAt = 0;

      this->data.results[0] = this->_nextData.results[0] = 1;
      this->data.results[1] = this->_nextData.results[1] = 1;
      this->data.results[2] = this->_nextData.results[2] = 1;
      this->data.results[3] = this->_nextData.results[3] = 1;
      this->data.results[4] = this->_nextData.results[4] = 1;
      this->data.results[5] = this->_nextData.results[5] = 1;
      this->data.results[6] = this->_nextData.results[6] = 1;
      this->data.results[7] = this->_nextData.results[7] = 1;
      this->data.results[8] = this->_nextData.results[8] = 1;
      this->data.resultIndex = 0;

      this->_nextData.dice = 0;
      this->_nextData.diceCount = 1;
      this->_nextData.brightness = STATE_BRIGHTNESS_2;
      this->_nextData.resultIndex = 0;
    }

    return;
  }

  // not an update loop, reset the flag
  this->isUpdateLoop = false;

  // sleep/wake logic
  if (this->data.mode == STATE_MODE_RESET) {
    // after a reset loop, signal that sleep is incoming with a sleep loop
    this->_nextData.mode = STATE_MODE_SLEEP;
    this->_pendingStateUpdate = true;
  } else if (this->data.mode == STATE_MODE_SLEEP) {
    // after a sleep loop, go to sleep. Upon wake, start a wake loop
    enterSleep();
    this->_nextData.mode = STATE_MODE_WAKE;
    this->_pendingStateUpdate = true;
  } else if (this->data.mode == STATE_MODE_WAKE) {
    // after a wake loop, always go to dice select - since only a button press
    // can wake from sleep
    this->_nextData.mode = STATE_MODE_SELECT_DICE;
    this->_pendingStateUpdate = true;
  }
}

void State::setModeResults() {
  this->_nextData.mode = STATE_MODE_RESULTS;
  this->_pendingStateUpdate = true;
}

void State::triggerButton(uint8_t buttonPress) {
  if (buttonPress == STATE_BUTTON_RESET) {
    if (this->data.mode != STATE_MODE_RESET) {
      this->_nextData.mode = STATE_MODE_RESET;
      this->_pendingStateUpdate = true;
    }
    return;
  }

  // any other button is considered an interaction
  this->_lastInteractionAt = millis();

  if (this->data.mode == STATE_MODE_RESULTS) {
    // if in results, on the button for the result is active
    if (this->data.dice != buttonPress) {
      return;
    }

    // Cycle through the results each time the button is pressed.
    // Even though resultIndex is a zero-based index, we allow it to go one
    // over. The last value (resultIndex == diceCount) is when we show the sum
    // of all of the rolls
    if (this->data.resultIndex == this->data.diceCount) {
      this->_nextData.resultIndex = 0;
    } else {
      this->_nextData.resultIndex = this->data.resultIndex + 1;
    }

    this->_pendingStateUpdate = true;
  } else if (this->data.mode != STATE_MODE_ROLLING) {
    this->_nextData.dice = buttonPress;

    // if a repeat press, increment the count through 9, then loop back to 1
    if (this->data.dice == buttonPress && this->data.diceCount < 9) {
      this->_nextData.diceCount = this->data.diceCount + 1;
    } else {
      this->_nextData.diceCount = 1;
    }

    // this is usually triggered right out of sleep mode.
    // update everything else, but let the sleep/wake logic handle `mode`
    if (this->data.mode != STATE_MODE_WAKE &&
        this->data.mode != STATE_MODE_SLEEP) {
      this->_nextData.mode = STATE_MODE_SELECT_DICE;
    }
    this->_pendingStateUpdate = true;
  }
}

void State::triggerRoll() {
  if (this->data.mode != STATE_MODE_SELECT_DICE) {
    return;
  }

  // if this is the first roll, start a timer
  if (this->_lastRolledAt == 0) {
    this->_lastRolledAt = micros();
    this->_lastInteractionAt = millis();
    return;
  }

  // if this is a second roll, debounce.
  // anything greater is considered 2 spins
  unsigned long diff = micros() - this->_lastRolledAt;
  if (diff < STATE_ROLL_DEBOUNCE) {
    return;
  }

  // only do the seed once
  if (!this->seedComplete) {
    this->seedComplete = true;
    randomSeed(diff);
  }

  this->_lastInteractionAt = millis();
  this->_lastRolledAt = 0;

  // +1 because `random` is non inclusive of the `max` value
  uint8_t diceCountMax = this->getDiceCount() + 1;
  for (uint8_t i = 0; i < this->data.diceCount; i++) {
    this->_nextData.results[i] = random(1, diceCountMax);
  }

  this->_nextData.mode = STATE_MODE_ROLLING;
  this->_pendingStateUpdate = true;
}

uint8_t State::getDiceCount() {
  switch (this->data.dice) {
  case STATE_BUTTON_4:
    return 4;
  case STATE_BUTTON_6:
    return 6;
  case STATE_BUTTON_8:
    return 8;
  case STATE_BUTTON_10:
    return 10;
  case STATE_BUTTON_12:
    return 12;
  case STATE_BUTTON_100:
    return 100;
  case STATE_BUTTON_20:
  default:
    return 20;
  }
}