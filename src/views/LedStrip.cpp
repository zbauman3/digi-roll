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

void LedStrip::clear() { SNX4HC595_sendByte(&this->config, 0b00000000); }

void LedStrip::loop() {
  if (this->state->isUpdateLoop) {
    if (this->state->isModeReset) {
      this->actionSetIdle(true);
      this->clear();
    } else if (this->state->isModeTest) {
      this->actionChange(LED_STRIP_ACTION_WIPE);
    } else if (this->state->isModeSelectDice) {
      this->disp = (1 << this->state->data.dice);
      this->actionChange(LED_STRIP_ACTION_BYTE, true);
    } else {
      this->actionSetIdle();
    }
  }

  this->runCoroutine();
}

int LedStrip::runCoroutine() {
  COROUTINE_LOOP() {
    if (this->actionIs(LED_STRIP_ACTION_WIPE)) {
      this->clear();

      for (this->i = 0; this->i < 7; this->i++) {
        SNX4HC595_sendByte(&this->config, (1 << this->i));
        COROUTINE_DELAY(150);
      }

      this->clear();
      this->actionSetIdle();
    } else if (this->actionIs(LED_STRIP_ACTION_BYTE)) {
      SNX4HC595_sendByte(&this->config, this->disp);
      this->actionSetIdle();
    }

    COROUTINE_YIELD();
  }
}