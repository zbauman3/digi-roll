#ifndef _ZANE_SNX4HC595_
#define _ZANE_SNX4HC595_

#include <Arduino.h>

typedef struct {
  uint8_t sclk;
  uint8_t rclk;
  uint8_t data;
} SNX4HC595Config;

void SNX4HC595_sendData(SNX4HC595Config *config);

void SNX4HC595_showData(SNX4HC595Config *config);

void SNX4HC595_sendByte(SNX4HC595Config *config, uint8_t value);

void SNX4HC595_clear(SNX4HC595Config *config);

void SNX4HC595_setup(SNX4HC595Config *config);

#endif