#ifndef Z_MatrixDisplay
#define Z_MatrixDisplay

#include "Adafruit_LEDBackpack.h"
#include <AceRoutine.h>
#include <Adafruit_GFX.h>
#include <Arduino.h>

using namespace ace_routine;

#define MATRIX_ACTION_IDLE 0
#define MATRIX_ACTION_ROLL 1

class MatrixDisplay : public Coroutine {
private:
  Adafruit_7segment matrix;
  uint8_t i;
  uint8_t j;
  uint8_t action;

public:
  MatrixDisplay();
  void begin();
  void rollDice();
  int runCoroutine() override;
};

#endif