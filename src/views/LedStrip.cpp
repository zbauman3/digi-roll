#include "./LedStrip.h"

LedStrip::LedStrip(State *_state) {
  this->state = _state;
  this->config = {
      .sclk = 10,
      .rclk = 8,
      .data = 9,
  };
}

void LedStrip::begin() { SNX4HC595_setup(&this->config); }

void LedStrip::loop() {
  // if update loop and either starting a roll or changing dice, reset.
  if (this->state->isUpdateLoop &&
      (this->state->isModeRolling ||
       (this->state->isModeSelectDice && this->state->data.diceCount == 1))) {
    this->reset();
  }

  this->runCoroutine();
}

int LedStrip::runCoroutine() {
  COROUTINE_LOOP() {
    if (this->state->isUpdateLoop) {
      if (this->state->isModeResults || this->state->isModeRolling) {
        SNX4HC595_sendByte(&this->config, (1 << this->state->data.dice));
      } else if (this->state->isModeReset) {
        // clear
        SNX4HC595_sendByte(&this->config, 0b00000000);
      }
    }

    // flash
    if (this->state->isModeSelectDice) {
      SNX4HC595_sendByte(&this->config, (1 << this->state->data.dice));
      COROUTINE_DELAY(500);
      SNX4HC595_sendByte(&this->config, 0b00000000);
      COROUTINE_DELAY(500);
    }

    COROUTINE_YIELD();
  }
}