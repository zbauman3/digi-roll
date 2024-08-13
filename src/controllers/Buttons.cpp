#include "./Buttons.h"

Buttons *isrButtons;
ISR(BUTTONS_INT_VECT) { isrButtons->handleInterrupt(); }

Buttons::Buttons(State *_state) { this->state = _state; }

void Buttons::begin() {
  isrButtons = this;

  pinMode(BUTTONS_0_PIN, INPUT);
  pinMode(BUTTONS_1_PIN, INPUT);
  pinMode(BUTTONS_2_PIN, INPUT);
  pinMode(BUTTONS_INT_PIN, INPUT_PULLUP);

  GIMSK |= (1 << BUTTONS_GIMSK);
  PCMSK0 |= (1 << BUTTONS_PCMSK0);
}

void Buttons::handleInterrupt() {
  if (digitalRead(BUTTONS_INT_PIN) == LOW) {
    this->lastPressed = (digitalRead(BUTTONS_0_PIN) * 1) +
                        (digitalRead(BUTTONS_1_PIN) * 2) +
                        (digitalRead(BUTTONS_2_PIN) * 4);
  } else {
    if (this->lastPressed == 7) {
      this->state->setModeReset();
    } else if (this->lastPressed != STATE_BUTTON_NONE) {
      this->state->setModeSelectDice(this->lastPressed);
    }

    this->lastPressed = STATE_BUTTON_NONE;
  }
}