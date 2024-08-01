#include "./LedStrip.h"

LedStrip::LedStrip() {
  this->config = {
      .sclk = 10,
      .rclk = 8,
      .data = 9,
  };
}

void LedStrip::begin() { SNX4HC595_setup(&this->config); }

void LedStrip::wipe() { this->actionChange(LED_STRIP_ACTION_WIPE); }

void LedStrip::clear() { SNX4HC595_sendByte(&this->config, 0b00000000); }

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
    }

    COROUTINE_YIELD();
  }
}