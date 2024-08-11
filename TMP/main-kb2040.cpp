#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "Adafruit_LEDBackpack.h"
#include <Adafruit_GFX.h>

arduino::MbedI2C Wire2 = arduino::MbedI2C(12, 13);
Adafruit_7segment matrix = Adafruit_7segment();

auto test = Wire;

void setup() {
  Wire2.begin();
  matrix.begin(0x70, &Wire2);

  uint8_t i;
  uint8_t j;

  matrix.clear();
  matrix.writeDisplay();

  for (j = 0; j < 5; j++) {
    if (j == 2) {
      j++;
    }

    for (i = 0; i < 6; i++) {
      matrix.writeDigitRaw(j, (1 << i));
      matrix.writeDisplay();
      delay(60);
    }

    matrix.writeDigitRaw(j, 1);
    matrix.writeDisplay();
    delay(60);

    matrix.writeDigitRaw(j, 0);
    matrix.writeDisplay();
  }
}

void loop() { delay(10); }