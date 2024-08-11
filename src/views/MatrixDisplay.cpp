#include "./MatrixDisplay.h"

MatrixDisplay::MatrixDisplay(State *_state) {
  this->state = _state;
  this->matrix = Adafruit_7segment();
};

void MatrixDisplay::begin() {
  this->matrix.begin(0x70);
  this->matrix.setBrightness(2);
}

void MatrixDisplay::rollDice() { this->actionChange(MATRIX_ACTION_ROLL); }

void MatrixDisplay::loop() {
  if (this->state->isUpdateLoop) {
    if (this->state->isModeReset) {
      this->actionSetIdle(true);
      this->matrix.clear();
      this->matrix.writeDisplay();
    } else if (this->state->isModeTest) {
      this->actionChange(MATRIX_ACTION_ROLL);
    } else {
      this->actionSetIdle();
    }
  }

  this->runCoroutine();
}

int MatrixDisplay::runCoroutine() {
  COROUTINE_LOOP() {
    if (this->actionIs(MATRIX_ACTION_ROLL)) {
      this->matrix.clear();
      this->matrix.writeDisplay();

      for (this->j = 0; this->j < 5; this->j++) {
        if (this->j == 2) {
          this->j++;
        }

        for (this->i = 0; this->i < 6; this->i++) {
          this->matrix.writeDigitRaw(this->j, (1 << this->i));
          this->matrix.writeDisplay();
          COROUTINE_DELAY(60);
        }

        this->matrix.writeDigitRaw(this->j, 1);
        this->matrix.writeDisplay();
        COROUTINE_DELAY(60);

        this->matrix.writeDigitRaw(this->j, 0);
        this->matrix.writeDisplay();
      }

      this->actionSetIdle();
    }

    COROUTINE_YIELD();
  }
}