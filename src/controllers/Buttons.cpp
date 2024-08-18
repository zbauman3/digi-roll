#include "./Buttons.h"

Buttons *isrButtons;
ISR(PCINT0_vect) { isrButtons->handleInterrupt(); }

Buttons::Buttons(State *_state) {
  this->state = _state;
  isrButtons = this;
}

void Buttons::begin() {
  pinMode(5, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT_PULLUP);

  GIMSK |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT7);
}

void Buttons::loop() {
  if (!this->didInterrupt) {
    return;
  }
  this->didInterrupt = false;

  unsigned long now = millis();
  if (now - this->lastPressedAt < 200) {
    return;
  }

  this->lastPressedAt = now;
  this->state->triggerButton(this->lastPressed[0] + (this->lastPressed[1] * 2) +
                             (this->lastPressed[2] * 4));
};

void Buttons::handleInterrupt() {
  if (!(PINA & _BV(PINA7))) {
    this->didInterrupt = true;
    this->lastPressed[0] = (PINA & _BV(PINA5)) ? 1 : 0;
    this->lastPressed[1] = (PINB & _BV(PINB1)) ? 1 : 0;
    this->lastPressed[2] = (PINB & _BV(PINB2)) ? 1 : 0;
  }
}