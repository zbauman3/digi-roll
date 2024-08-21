
#include "./HallEffect.h"

HallEffect *isrHallEffect;
ISR(PCINT1_vect) { isrHallEffect->handleInterrupt(); }

HallEffect::HallEffect(State *_state) {
  this->state = _state;
  isrHallEffect = this;
}

void HallEffect::begin() {
  pinMode(PIN_HALL_INT, INPUT_PULLUP);
  PIN_HALL_INT_SETUP();
}

void HallEffect::loop() {
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