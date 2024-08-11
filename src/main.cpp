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

void setup() {
  pinMode(HAL_PIN, INPUT_PULLUP);

  matrixDisplay.begin();
  ledStrip.begin();

  matrixDisplay.rollDice();
  ledStrip.wipe();
}

void loop() {
  state.loop();

  uint8_t hallActive = !digitalRead(HAL_PIN);
  if (hallActive) {
    state.setModeTest();
  }

  matrixDisplay.loop();
  ledStrip.loop();

  // bool btn0 = digitalRead(BTN_A0) == LOW;
  // bool btn1 = digitalRead(BTN_A1) == LOW;
  // bool btn2 = digitalRead(BTN_A2) == LOW;

  // if (btn0 || btn1 || btn2) {
  //   uint8_t data = 0;

  //   if (btn2) {
  //     if (btn1) {
  //       if (btn0) {
  //         data = 0b00000001;
  //       } else {
  //         data = 0b00000010;
  //       }
  //     } else if (btn0) {
  //       data = 0b00000100;
  //     } else {
  //       data = 0b00001000;
  //     }
  //   } else if (btn1) {
  //     if (btn0) {
  //       data = 0b00010000;
  //     } else {
  //       data = 0b00100000;
  //     }
  //   } else {
  //     data = 0b01000000;
  //   }
  //   SNX4HC595_sendByte(&config595, data);
  // }

  // SNX4HC595_sendByte(&config595, 0b01000000);
  // delay(1000);
  // SNX4HC595_sendByte(&config595, 0b00000000);
  // delay(1000);
  // SNX4HC595_sendByte(&config595, 0b00000001);
  // delay(1000);
  // SNX4HC595_sendByte(&config595, 0b00000000);
  // delay(1000);

  // // try to print a number thats too long
  // matrix.print(10000, DEC);
  // matrix.writeDisplay();
  // delay(1000);

  // // print a hex number
  // matrix.print(0xBEEF, HEX);
  // matrix.writeDisplay();
  // delay(1000);

  // // print a floating point
  // matrix.print(12.34);
  // matrix.writeDisplay();
  // delay(1000);

  // // print a string message
  // matrix.print("7SEG");
  // matrix.writeDisplay();
  // delay(10000);

  // // print with print/println
  // for (uint16_t counter = 0; counter < 9999; counter++) {
  //   matrix.println(counter);
  //   matrix.writeDisplay();
  //   delay(10);
  // }

  // // method #2 - draw each digit
  // uint16_t blinkcounter = 0;
  // boolean drawDots = false;
  // for (uint16_t counter = 0; counter < 9999; counter++) {
  //   matrix.writeDigitNum(0, (counter / 1000), drawDots);
  //   matrix.writeDigitNum(1, (counter / 100) % 10, drawDots);
  //   matrix.drawColon(drawDots);
  //   matrix.writeDigitNum(3, (counter / 10) % 10, drawDots);
  //   matrix.writeDigitNum(4, counter % 10, drawDots);

  //   blinkcounter += 50;
  //   if (blinkcounter < 500) {
  //     drawDots = false;
  //   } else if (blinkcounter < 1000) {
  //     drawDots = true;
  //   } else {
  //     blinkcounter = 0;
  //   }
  //   matrix.writeDisplay();
  //   delay(10);
  // }
}