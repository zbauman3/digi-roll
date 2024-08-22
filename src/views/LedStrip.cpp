#include "./LedStrip.h"

LedStrip::LedStrip(State *_state) {
  this->state = _state;
  this->sclk = PIN_LED_SCLK;
  this->rclk = PIN_LED_RCLK;
  this->data = PIN_LED_DATA;
}

void LedStrip::sendByte(uint8_t value) {
  uint8_t mask;
  int8_t i;

  // could use the USI for this, but it's being used for I2C
  for (i = 7; i >= 0; i--) {
    mask = value & (1 << i);
    if (mask == 0) {
      digitalWrite(this->data, LOW);
    } else {
      digitalWrite(this->data, HIGH);
    }
    digitalWrite(this->sclk, HIGH);
    digitalWrite(this->sclk, LOW);
  }
  digitalWrite(this->rclk, HIGH);
  digitalWrite(this->rclk, LOW);
  // always back to LOW, to save power
  digitalWrite(this->data, LOW);
}

void LedStrip::begin() {
  pinMode(this->sclk, OUTPUT);
  pinMode(this->rclk, OUTPUT);
  pinMode(this->data, OUTPUT);

  digitalWrite(this->sclk, LOW);
  digitalWrite(this->rclk, LOW);
  digitalWrite(this->data, LOW);

  this->sendByte(0b00000000);
}

void LedStrip::loop() {
  if (this->state->isUpdateLoop) {
    // reset if update loop and either starting a roll or changing dice
    if (this->state->data.mode == STATE_MODE_ROLLING ||
        (this->state->data.mode == STATE_MODE_SELECT_DICE &&
         this->state->data.diceCount == 1)) {
      this->reset();
    } else if (this->state->data.mode == STATE_MODE_SLEEP) {
      this->sendByte(0b00000000);
    }
  }

  this->runCoroutine();
}

int LedStrip::runCoroutine() {
  COROUTINE_LOOP() {
    if (this->state->isUpdateLoop &
        (this->state->data.mode == STATE_MODE_RESULTS ||
         this->state->data.mode == STATE_MODE_ROLLING)) {
      this->sendByte((1 << this->state->data.dice));
    }

    // flash
    if (this->state->data.mode == STATE_MODE_SELECT_DICE) {
      this->sendByte((1 << this->state->data.dice));
      COROUTINE_DELAY(500);
      this->sendByte(0b00000000);
      COROUTINE_DELAY(500);
    }

    COROUTINE_YIELD();
  }
}