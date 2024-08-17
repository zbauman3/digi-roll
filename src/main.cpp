#include "controllers/Buttons.h"
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

State state;
MatrixDisplay matrixDisplay(&state);
LedStrip ledStrip(&state);
Buttons buttons(&state);

void setup() {
  pinMode(HAL_PIN, INPUT_PULLUP);

  buttons.begin();
  matrixDisplay.begin();
  ledStrip.begin();
  state.setModeReset();

  sei();
}

void loop() {
  buttons.loop();
  state.loop();

  // uint8_t hallActive = !digitalRead(HAL_PIN);
  // if (hallActive) {
  //   state.setModeTest();
  // }

  matrixDisplay.loop();
  ledStrip.loop();
}