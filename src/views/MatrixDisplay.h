#ifndef Z_MatrixDisplay
#define Z_MatrixDisplay

#include "Adafruit_LEDBackpack.h"
#include "models/State.h"
#include <AceRoutine.h>
#include <Adafruit_GFX.h>

using namespace ace_routine;

class MatrixDisplay : public Coroutine {
private:
  Adafruit_7segment matrix;
  uint8_t i;
  uint8_t j;
  State *state;
  uint8_t _lastBrightness;

public:
  MatrixDisplay(State *_state);
  void begin();
  int runCoroutine() override;
};

#endif