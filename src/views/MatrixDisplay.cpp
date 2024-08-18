#include "./MatrixDisplay.h"

MatrixDisplay::MatrixDisplay(State *_state) {
  this->state = _state;
  this->matrix = Adafruit_7segment();
};

void MatrixDisplay::begin() {
  this->matrix.begin(0x70);
  this->matrix.setBrightness(this->state->data.brightness);
  this->_lastBrightness = this->state->data.brightness;
}

int MatrixDisplay::runCoroutine() {
  COROUTINE_LOOP() {
    if (this->state->isUpdateLoop) {
      if (this->_lastBrightness != this->state->data.brightness) {
        this->_lastBrightness = this->state->data.brightness;
        this->matrix.setBrightness(this->state->data.brightness);
        COROUTINE_YIELD();
      }

      if (this->state->isModeSelectDice) {
        this->matrix.clear();

        uint8_t diceCount = this->state->getDiceCount();
        if (diceCount < 10) {
          this->matrix.writeDigitNum(4, diceCount);
        } else if (diceCount < 100) {
          this->matrix.print("  ");
          this->matrix.printNumber(diceCount, 10);
        } else {
          this->matrix.writeDigitAscii(4, 'P');
        }

        this->matrix.drawColon(true);
        this->matrix.writeDigitNum(0, this->state->data.diceCount);
        this->matrix.writeDigitAscii(1, 'd');

        this->matrix.writeDisplay();
      } else if (this->state->isModeRolling) {
        this->matrix.clear();

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

        this->state->setModeResults();
      } else if (this->state->isModeResults) {
        this->matrix.clear();

        uint8_t thisResult =
            this->state->data.results[this->state->data.resultIndex];
        if (thisResult < 10) {
          this->matrix.writeDigitNum(4, thisResult);
        } else if (thisResult < 100) {
          this->matrix.print("  ");
          this->matrix.printNumber(thisResult, 10);
        } else {
          this->matrix.print(" ");
          this->matrix.printNumber(thisResult, 10);
        }

        this->matrix.drawColon(true);
        this->matrix.writeDigitNum(0, this->state->data.resultIndex + 1);
        this->matrix.writeDigitAscii(1, 'd');

        this->matrix.writeDisplay();
      } else if (this->state->isModeReset) {
        this->matrix.clear();
        this->matrix.writeDisplay();
      }
    }

    COROUTINE_YIELD();
  }
}