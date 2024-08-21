#include "./LedStrip.h"

LedStrip::LedStrip(State *_state) {
  this->state = _state;
  this->config = {
      .sclk = PIN_LED_SCLK,
      .rclk = PIN_LED_RCLK,
      .data = PIN_LED_DATA,
  };
}

void LedStrip::begin() { SNX4HC595_setup(&this->config); }

void LedStrip::loop() {
  // if update loop and either starting a roll or changing dice, reset.
  if (this->state->isUpdateLoop &&
      (this->state->data.mode == STATE_MODE_ROLLING ||
       (this->state->data.mode == STATE_MODE_SELECT_DICE &&
        this->state->data.diceCount == 1))) {
    this->reset();
  }

  this->runCoroutine();
}

int LedStrip::runCoroutine() {
  COROUTINE_LOOP() {
    if (this->state->isUpdateLoop) {
      if (this->state->data.mode == STATE_MODE_RESULTS ||
          this->state->data.mode == STATE_MODE_ROLLING) {
        SNX4HC595_sendByte(&this->config, (1 << this->state->data.dice));
      } else if (this->state->data.mode == STATE_MODE_RESET) {
        // clear
        SNX4HC595_sendByte(&this->config, 0b00000000);
      }
    }

    // flash
    if (this->state->data.mode == STATE_MODE_SELECT_DICE) {
      SNX4HC595_sendByte(&this->config, (1 << this->state->data.dice));
      COROUTINE_DELAY(500);
      SNX4HC595_sendByte(&this->config, 0b00000000);
      COROUTINE_DELAY(500);
    }

    COROUTINE_YIELD();
  }
}