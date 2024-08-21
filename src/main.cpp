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
  pinMode(11, OUTPUT); // reset should be output low
  digitalWrite(11, LOW);

  sei();
  buttons.begin();
  hallEffect.begin();
  matrixDisplay.begin();
  ledStrip.begin();
}

void loop() {
  buttons.loop();
  hallEffect.loop();

  state.loop();

  matrixDisplay.loop();
  ledStrip.loop();
}