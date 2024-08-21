
#include "./sleep.h"

SNX4HC595Config config = {
    .sclk = 10,
    .rclk = 8,
    .data = 9,
};

void enterSleep() {
  SNX4HC595_sendByte(&config, 0b10101010);
  delay(500);
  SNX4HC595_sendByte(&config, 0b00000000);

  ADCSRA &= ~(_BV(ADEN));
  ACSR |= _BV(ACD);
  power_adc_disable();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();
  sleep_disable();

  SNX4HC595_sendByte(&config, 0b11111111);
  delay(500);
  SNX4HC595_sendByte(&config, 0b00000000);
}
