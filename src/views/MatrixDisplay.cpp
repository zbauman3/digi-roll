#include "./MatrixDisplay.h"

MatrixDisplay::MatrixDisplay() { this->matrix = Adafruit_7segment(); };

void MatrixDisplay::begin() {
  this->matrix.begin(0x70);
  this->matrix.setBrightness(2);
}

void MatrixDisplay::rollDice() { this->actionChange(MATRIX_ACTION_ROLL); }

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