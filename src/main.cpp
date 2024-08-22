#include "controllers/Buttons.h"
#include "controllers/HallEffect.h"
#include "models/State.h"
#include "views/LedStrip.h"
#include "views/MatrixDisplay.h"

State state;
MatrixDisplay matrixDisplay(&state);
LedStrip ledStrip(&state);
Buttons buttons(&state);
HallEffect hallEffect(&state);

void setup() {
  // reset should be output low
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);

  sei();
  buttons.begin();
  hallEffect.begin();
  matrixDisplay.begin();
  ledStrip.begin();
}

void loop() {
  // controllers
  buttons.loop();
  hallEffect.loop();

  // models
  state.loop();

  // views
  matrixDisplay.loop();
  ledStrip.loop();
}