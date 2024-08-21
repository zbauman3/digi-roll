#ifndef Z_PINS_H
#define Z_PINS_H

// ATMEL ATTINY84 / ARDUINO - COUNTERCLOCKWISE
//
//                           +-\/-+
//                     VCC  1|    |14  GND
//  HALL       (D  0)  PB0  2|    |13  PA0  (D 10)        LED
//  btn        (D  1)  PB1  3|    |12  PA1  (D  9)        LED
//             (D 11)  PB3  4|    |11  PA2  (D  8)        LED
//  btn        (D  2)  PB2  5|    |10  PA3  (D  7)        NPN BPJ
//  btn        (D  3)  PA7  6|    |9   PA4  (D  6)        i2c
//  i2c        (D  4)  PA6  7|    |8   PA5  (D  5)        btn
//

#define PIN_HALL_INT 0
#define PIN_HALL_INT_SETUP()                                                   \
  GIMSK |= (1 << PCIE1);                                                       \
  PCMSK1 |= (1 << PCINT8)
#define PIN_HALL_ENABLE 7

#define PIN_BTN_0 5
#define PIN_BTN_1 1
#define PIN_BTN_2 2
#define PIN_BTN_0_READ() ((PINA & _BV(PINA5)) ? 1 : 0)
#define PIN_BTN_1_READ() ((PINB & _BV(PINB1)) ? 1 : 0)
#define PIN_BTN_2_READ() ((PINB & _BV(PINB2)) ? 1 : 0)

#define PIN_BTN_INT 3
#define PIN_BTN_INT_SETUP()                                                    \
  GIMSK |= (1 << PCIE0);                                                       \
  PCMSK0 |= (1 << PCINT7)

#define PIN_LED_SCLK 10
#define PIN_LED_RCLK 8
#define PIN_LED_DATA 9

#endif // Z_PINS_H