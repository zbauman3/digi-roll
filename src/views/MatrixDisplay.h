#ifndef Z_MatrixDisplay
#define Z_MatrixDisplay

#include "Adafruit_LEDBackpack.h"
#include "util/CoroutineAction.h"
#include <Adafruit_GFX.h>

#define MATRIX_ACTION_ROLL 1

class MatrixDisplay : public CoroutineAction {
private:
  Adafruit_7segment matrix;
  uint8_t i;
  uint8_t j;

public:
  MatrixDisplay();
  void begin();
  void rollDice();
  int runCoroutine() override;
};

#endif