#include "./SNX4HC595.h"

void SNX4HC595_sendData(SNX4HC595Config *config) {
  digitalWrite(config->sclk, HIGH);
  digitalWrite(config->sclk, LOW);
}

void SNX4HC595_showData(SNX4HC595Config *config) {
  digitalWrite(config->rclk, HIGH);
  digitalWrite(config->rclk, LOW);
}

void SNX4HC595_sendByte(SNX4HC595Config *config, uint8_t value) {
  uint8_t mask;
  int8_t i;

  for (i = 7; i >= 0; i--) {
    mask = value & (1 << i);
    if (mask == 0) {
      digitalWrite(config->data, LOW);
    } else {
      digitalWrite(config->data, HIGH);
    }
    SNX4HC595_sendData(config);
  }
  SNX4HC595_showData(config);
  digitalWrite(config->data, LOW);
}

void SNX4HC595_clear(SNX4HC595Config *config) { SNX4HC595_sendByte(config, 0); }

void SNX4HC595_setup(SNX4HC595Config *config) {
  pinMode(config->sclk, OUTPUT);
  pinMode(config->rclk, OUTPUT);
  pinMode(config->data, OUTPUT);

  digitalWrite(config->sclk, LOW);
  digitalWrite(config->rclk, LOW);
  digitalWrite(config->data, LOW);

  SNX4HC595_clear(config);
}