#include "./sleep.h"

void enterSleep() {
  // a hack to make pressing `reset` not wake the device when releasing
  delay(750);
  ADCSRA &= ~(_BV(ADEN));
  ACSR |= _BV(ACD);
  power_adc_disable();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();
  sleep_disable();
}
