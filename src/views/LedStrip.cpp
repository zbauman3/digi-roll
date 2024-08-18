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

int LedStrip::runCoroutine() {
  COROUTINE_LOOP() {
    if (this->state->isUpdateLoop) {
      if (this->state->isModeSelectDice || this->state->isModeResults) {
        SNX4HC595_sendByte(&this->config, (1 << this->state->data.dice));
      } else if (this->state->isModeReset) {
        // clear
        SNX4HC595_sendByte(&this->config, 0b00000000);
      }
    }

    COROUTINE_YIELD();
  }
}