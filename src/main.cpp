#include "controllers/Buttons.h"
#include "controllers/HallEffect.h"
#include "models/State.h"
#include "views/LedStrip.h"
#include "views/MatrixDisplay.h"
#include <Arduino.h>
#include <Wire.h>

State state;
MatrixDisplay matrixDisplay(&state);
LedStrip ledStrip(&state);
Buttons buttons(&state);
HallEffect hallEffect(&state);

void setup() {
  sei();
  buttons.begin();
  hallEffect.begin();
  matrixDisplay.begin();
  ledStrip.begin();
  state.setModeReset();
}

void loop() {
  buttons.loop();
  hallEffect.loop();

  state.loop();

  matrixDisplay.loop();
  ledStrip.loop();
}