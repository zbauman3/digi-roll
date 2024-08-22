#ifndef Z_PINS_H
#define Z_PINS_H

// ATMEL ATTINY84 / ARDUINO - COUNTERCLOCKWISE
//
//                           +-\/-+
//                     VCC  1|    |14  GND
//  HALL INT   (D  0)  PB0  2|    |13  PA0  (D 10)        LED SCK
//  BTN 1      (D  1)  PB1  3|    |12  PA1  (D  9)        LED DATA
//             (D 11)  PB3  4|    |11  PA2  (D  8)        LED RCLK
//  BTN 2      (D  2)  PB2  5|    |10  PA3  (D  7)        NPN BPJ
//  BTN INT    (D  3)  PA7  6|    |9   PA4  (D  6)        I2C SCL
//  I2C SDA    (D  4)  PA6  7|    |8   PA5  (D  5)        BTN 0
//

#define PIN_HALL_INT 0
#define PIN_HALL_INT_VECT PCINT1_vect
// setup the interrupt for the hall effect sensor
#define PIN_HALL_INT_SETUP()                                                   \
  GIMSK |= (1 << PCIE1);                                                       \
  PCMSK1 |= (1 << PCINT8)
// check if the hall effect sensor is currently triggering
#define PIN_HALL_INT_IS_ACTIVE() (!(PINB & _BV(PINB0)))
#define PIN_HALL_ENABLE 7

#define PIN_BTN_0 5
#define PIN_BTN_1 1
#define PIN_BTN_2 2
// read the buttons without using the arduino libs
#define PIN_BTN_0_READ() ((PINA & _BV(PINA5)) ? 1 : 0)
#define PIN_BTN_1_READ() ((PINB & _BV(PINB1)) ? 1 : 0)
#define PIN_BTN_2_READ() ((PINB & _BV(PINB2)) ? 1 : 0)

#define PIN_BTN_INT 3
#define PIN_BTN_INT_VECT PCINT0_vect
// setup the interrupt for the buttons
#define PIN_BTN_INT_SETUP()                                                    \
  GIMSK |= (1 << PCIE0);                                                       \
  PCMSK0 |= (1 << PCINT7)
// check if the buttons are currently triggering
#define PIN_BTN_INT_IS_PRESSING() (!(PINA & _BV(PINA7)))

#define PIN_LED_SCLK 10
#define PIN_LED_RCLK 8
#define PIN_LED_DATA 9

#endif // Z_PINS_H