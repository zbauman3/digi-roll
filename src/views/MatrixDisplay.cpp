#include "./MatrixDisplay.h"

MatrixDisplay::MatrixDisplay(State *_state) {
  this->state = _state;
  this->matrix = Adafruit_7segment();
};

void MatrixDisplay::begin() {
  this->matrix.begin(MATRIX_DISPLAY_ADDR);
  this->matrix.setBrightness(this->state->data.brightness);
  this->_lastBrightness = this->state->data.brightness;
}

void MatrixDisplay::loop() {
  if (this->state->isUpdateLoop) {
    if (this->state->data.mode == STATE_MODE_SLEEP) {
      // sleep the display
      uint8_t buffer[1] = {0x20};
      Wire.beginTransmission(MATRIX_DISPLAY_ADDR);
      while (Wire.write(buffer, 1) != 1) {
        delay(1);
      }
      Wire.endTransmission(true);
    } else if (this->state->data.mode == STATE_MODE_WAKE) {
      // wake the display
      uint8_t buffer[1] = {0x21};
      Wire.beginTransmission(MATRIX_DISPLAY_ADDR);
      while (Wire.write(buffer, 1) != 1) {
        delay(1);
      }
      Wire.endTransmission(true);
    }
  }

  this->runCoroutine();
}

int MatrixDisplay::runCoroutine() {
  COROUTINE_LOOP() {
    if (this->state->isUpdateLoop) {
      if (this->_lastBrightness != this->state->data.brightness) {
        this->_lastBrightness = this->state->data.brightness;
        this->matrix.setBrightness(this->state->data.brightness);
        COROUTINE_YIELD();
      }

      this->matrix.clear();

      if (this->state->data.mode == STATE_MODE_SELECT_DICE) {
        this->matrix.writeDigitNum(0, this->state->data.diceCount);
        this->matrix.writeDigitAscii(1, 'd');

        uint8_t diceCount = this->state->getDiceCount();
        switch (diceCount) {
        case 4:
        case 6:
        case 8:
          this->matrix.writeDigitNum(4, diceCount);
          break;
        case 10:
        case 12:
          this->matrix.writeDigitNum(3, 1);
          this->matrix.writeDigitNum(4, diceCount % 10);
          break;
        case 100:
          this->matrix.writeDigitAscii(4, 'P');
          break;
        case 20:
        default:
          this->matrix.writeDigitNum(3, 2);
          this->matrix.writeDigitNum(4, 0);
          break;
        }

        this->matrix.writeDisplay();
      } else if (this->state->data.mode == STATE_MODE_ROLLING) {
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
      } else if (this->state->data.mode == STATE_MODE_RESULTS) {
        this->matrix.writeDigitNum(0, this->state->data.resultIndex + 1, true);

        uint8_t currentResult =
            this->state->data.results[this->state->data.resultIndex];
        if (currentResult < 10) {
          this->matrix.writeDigitNum(4, currentResult);
        } else if (currentResult < 100) {
          uint8_t lsd = currentResult % 10;
          this->matrix.writeDigitNum(
              3, (uint8_t)((uint8_t)(currentResult - lsd) / (uint8_t)10));
          this->matrix.writeDigitNum(4, lsd);
        } else {
          this->matrix.writeDigitNum(1, 1);
          this->matrix.writeDigitNum(3, 0);
          this->matrix.writeDigitNum(4, 0);
        }

        this->matrix.writeDisplay();
      } else if (this->state->data.mode == STATE_MODE_RESET) {
        this->matrix.writeDisplay();
      }
    }

    COROUTINE_YIELD();
  }
}