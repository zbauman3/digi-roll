#ifndef Z_MatrixDisplay
#define Z_MatrixDisplay

#include "Adafruit_LEDBackpack.h"
#include "models/State.h"
#include "util/CoroutineAction.h"
#include <Adafruit_GFX.h>

#define MATRIX_ACTION_ROLL 1
#define MATRIX_ACTION_SELECT_DICE 2

class MatrixDisplay : public CoroutineAction {
private:
  Adafruit_7segment matrix;
  uint8_t i;
  uint8_t j;
  State *state;
  uint8_t _lastBrightness;
  bool updateBrightness;

public:
  MatrixDisplay(State *_state);
  void begin();
  void loop() override;
  int runCoroutine() override;
};

#endif