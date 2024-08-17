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

void MatrixDisplay::loop() {
  if (this->state->isUpdateLoop) {
    if (this->state->isModeReset) {
      this->actionSetIdle(true);
      this->matrix.clear();
      this->matrix.writeDisplay();
    } else if (this->state->isModeTest) {
      this->actionChange(MATRIX_ACTION_ROLL);
    } else if (this->state->isModeSelectDice) {
      this->actionChange(MATRIX_ACTION_SELECT_DICE);
    } else {
      this->actionSetIdle();
    }

    if (this->_lastBrightness != this->state->data.brightness) {
      this->_lastBrightness = this->state->data.brightness;
      this->updateBrightness = true;
    } else {
      this->updateBrightness = false;
    }
  }

  this->runCoroutine();
}

int MatrixDisplay::runCoroutine() {
  COROUTINE_LOOP() {
    if (this->updateBrightness) {
      this->matrix.setBrightness(this->state->data.brightness);
      COROUTINE_YIELD();
    }

    if (this->actionIs(MATRIX_ACTION_ROLL)) {
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

      this->actionSetIdle();
    } else if (this->actionIs(MATRIX_ACTION_SELECT_DICE) &&
               this->state->data.dice != STATE_DICE_NONE) {
      this->matrix.clear();
      this->matrix.writeDigitNum(0, this->state->data.diceCount);
      this->matrix.writeDigitAscii(1, 'd');

      switch (this->state->data.dice) {
      case 0:
        this->matrix.writeDigitNum(4, 4);
        break;
      case 1:
        this->matrix.writeDigitNum(4, 6);
        break;
      case 2:
        this->matrix.writeDigitNum(4, 8);
        break;
      case 3:
        this->matrix.writeDigitNum(3, 1);
        this->matrix.writeDigitNum(4, 0);
        break;
      case 4:
        this->matrix.writeDigitNum(3, 1);
        this->matrix.writeDigitNum(4, 2);
        break;
      case 5:
        this->matrix.writeDigitNum(3, 2);
        this->matrix.writeDigitNum(4, 0);
        break;
      case 6:
        this->matrix.writeDigitAscii(4, 'P');
        break;
      }
      this->matrix.writeDisplay();
    }

    COROUTINE_YIELD();
  }
}