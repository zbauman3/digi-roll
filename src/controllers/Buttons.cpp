#include "./Buttons.h"

Buttons *isrButtons;
ISR(PIN_BTN_INT_VECT) { isrButtons->handleInterrupt(); }

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

  uint8_t local_lastPressed[3] = {0, 0, 0};
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    local_lastPressed[0] = this->lastPressed[0];
    local_lastPressed[1] = this->lastPressed[1];
    local_lastPressed[2] = this->lastPressed[2];
  }

  // software debounce
  unsigned long now = millis();
  if (now - this->lastPressedAt < 200) {
    return;
  }

  this->lastPressedAt = now;

  // decode to decimal
  this->state->triggerButton(local_lastPressed[0] + (local_lastPressed[1] * 2) +
                             (local_lastPressed[2] * 4));
};

void Buttons::handleInterrupt() {
  if (PIN_BTN_INT_IS_PRESSING()) {
    this->didInterrupt = true;
    this->lastPressed[0] = PIN_BTN_0_READ();
    this->lastPressed[1] = PIN_BTN_1_READ();
    this->lastPressed[2] = PIN_BTN_2_READ();
  }
}