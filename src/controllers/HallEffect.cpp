
#include "./HallEffect.h"

HallEffect *isrHallEffect;
ISR(PCINT1_vect) { isrHallEffect->handleInterrupt(); }

HallEffect::HallEffect(State *_state) {
  this->state = _state;
  isrHallEffect = this;
}

void HallEffect::begin() {
  pinMode(PIN_HALL_INT, INPUT_PULLUP);
  pinMode(PIN_HALL_ENABLE, OUTPUT);
  digitalWrite(PIN_HALL_ENABLE, LOW);
  this->isEnabled = false;
  PIN_HALL_INT_SETUP();
}

void HallEffect::loop() {
  if (this->state->isUpdateLoop) {
    if (this->state->data.mode == STATE_MODE_SELECT_DICE) {
      if (!this->isEnabled) {
        digitalWrite(PIN_HALL_ENABLE, HIGH);
        this->isEnabled = true;
      }
    } else if (this->isEnabled) {
      digitalWrite(PIN_HALL_ENABLE, LOW);
      this->isEnabled = false;
    }
  }

  if (!this->didInterrupt) {
    return;
  }
  this->didInterrupt = false;
  this->state->triggerRoll();
}

void HallEffect::handleInterrupt() {
  if (!(PINB & _BV(PINB0))) {
    this->didInterrupt = true;
  }
}