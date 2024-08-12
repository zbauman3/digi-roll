#include "models/State.h"
#include "views/LedStrip.h"
#include "views/MatrixDisplay.h"
#include <Arduino.h>
#include <Wire.h>

// ATMEL ATTINY84 / ARDUINO
//
//                           +-\/-+
//                     VCC  1|    |14  GND
//  btn        (D  0)  PB0  2|    |13  PA0  (D 10)        LED
//  btn        (D  1)  PB1  3|    |12  PA1  (D  9)        LED
//             (D 11)  PB3  4|    |11  PA2  (D  8)
//  btn        (D  2)  PB2  5|    |10  PA3  (D  7)
//  btn        (D  3)  PA7  6|    |9   PA4  (D  6)        i2c
//  i2c        (D  4)  PA6  7|    |8   PA5  (D  5)        HAL
//

#define HAL_PIN 5
#define BTN_0 0
#define BTN_1 1
#define BTN_2 2
#define BTN_INT 3
uint8_t buttonState = 0;
bool buttonInt = false;

State state;
MatrixDisplay matrixDisplay(&state);
LedStrip ledStrip(&state);

void setup() {
  pinMode(HAL_PIN, INPUT_PULLUP);
  pinMode(BTN_0, INPUT);
  pinMode(BTN_1, INPUT);
  pinMode(BTN_2, INPUT);
  pinMode(BTN_INT, INPUT_PULLUP);

  GIMSK |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT7);
  sei();

  matrixDisplay.begin();
  ledStrip.begin();
  state.setModeReset();
}

void loop() {
  state.loop();

  uint8_t hallActive = !digitalRead(HAL_PIN);
  if (hallActive) {
    state.setModeTest();
  }

  if (buttonInt) {
    buttonInt = false;
    ledStrip.showByte(buttonState);
  }

  matrixDisplay.loop();
  ledStrip.loop();
}

ISR(PCINT0_vect) {
  if (digitalRead(BTN_INT) == LOW) {
    buttonState = (1 << ((digitalRead(BTN_0) * 1) + (digitalRead(BTN_1) * 2) +
                         (digitalRead(BTN_2) * 4)));
    buttonInt = true;
  }
}