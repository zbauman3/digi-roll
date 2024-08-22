#ifndef Z_MatrixDisplay
#define Z_MatrixDisplay

#include "Adafruit_LEDBackpack.h"
#include "models/State.h"
#include <AceRoutine.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

using namespace ace_routine;

#define MATRIX_DISPLAY_ADDR 0x70

class MatrixDisplay : public Coroutine {
private:
  Adafruit_7segment matrix;
  uint8_t i;
  uint8_t j;
  State *state;
  uint8_t _lastBrightness;

public:
  MatrixDisplay(State *_state);
  void loop();
  void begin();
  int runCoroutine() override;
};

#endif