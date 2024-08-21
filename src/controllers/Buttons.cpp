#include "./Buttons.h"

Buttons *isrButtons;
ISR(PCINT0_vect) { isrButtons->handleInterrupt(); }

Buttons::Buttons(State *_state) {
  this->state = _state;
  isrButtons = this;
}

void Buttons::begin() {
  pinMode(PIN_BTN_0, INPUT);
  pinMode(PIN_BTN_1, INPUT);
  pinMode(PIN_BTN_2, INPUT);
  pinMode(PIN_BTN_INT, INPUT_PULLUP);
  PIN_BTN_INT_SETUP();
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
    this->lastPressed[0] = PIN_BTN_0_READ();
    this->lastPressed[1] = PIN_BTN_1_READ();
    this->lastPressed[2] = PIN_BTN_2_READ();
  }
}