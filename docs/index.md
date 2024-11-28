---
layout: default
title: Home
---

# DigiRoll

DigiRoll is a digital dice roller, specifically targeting Dungeons And Dragons.

<img src="./assets/media/assembled/front-on.jpeg" class="img-full" />

## Hardware

> See the [Parts page](./parts) for specifics and URLs for the exact parts.

For this project, I had a few hardware goals in mind. I wanted to design and order the printed circuit board (PCB) myself, I wanted to do much of the electrical engineering myself, and I wanted the end circuit to be very power efficient.

Having never designed a PCB before this project, this was an excellent opportunity to learn the entire process from design to manufacturing. I used KiCad for the PCB design and [JLCPCB](https://jlcpcb.com/) for fabrication. Once I created the schematic in KiCad, transitioning to the PCB layout was pretty straight forward. By watching several YouTube tutorials, I was able to learn the steps and complete the design independently.

I wanted to handle as most of the electrical engineering myself, while still keeping the project's timeline reasonable. To save on time, I made a compromise by using breakout boards for the boost converter and the 7-segment display. If I do a second revision, I plan to design these components myself, which could potentially reduce the cost per unit while also allowing me to have most of the board assembled at the factory.

The main components of the board are an ATtiny84 MCU, a CD4532 8-bit encoder, and an SN74HC595 shift register. Initially, I considered using an ATtiny85, but its limited number of pins made it impractical unless I could use I2C for all peripherals, which wasn’t feasible. The shift register controls the LEDs for the buttons, while the encoder allows the device to support eight buttons using only four MCU pins (three for encoding the value and one for interrupts).

Designing the mechanism for “spinning” the dice was the next major challenge. A Hall effect sensor was an ideal choice for this purpose, but these sensors are notoriously power-hungry, conflicting with my goal of maximizing power efficiency. To address this issue, I powered the sensor through a GPIO pin on the MCU. This approach allowed me to disable the sensor when it wasn’t needed, conserving power during most of the device’s active period.

For the display, I selected an Adafruit breakout board featuring the HT16K33 driver. I used the [Adafruit_LEDBackpack](https://github.com/adafruit/Adafruit_LED_Backpack) library, which also simplified the integration process. However, the library didn’t include commands to put the HT16K33 into sleep mode, which was a necessary feature given its multi-milliamp power consumption. Fortunately, I was able to manually send sleep and wake instructions to the HT16K33.

The final major component was the boost converter. Although I could have integrated this into the circuit myself, my lack of experience with them and the desire to complete the project quickly led me to use another breakout board featuring the TPS61023. This allowed the device to be powered by just two AA batteries, even though the system required 5V. The efficiency of this setup is approximately 88%, which is acceptable but not optimal.

By combining these hardware choices with low-current LEDs and efficient software, I achieved a reasonable power profile. Using [Nordic's Power Profiler Kit II](https://www.nordicsemi.com/Products/Development-hardware/Power-Profiler-Kit-2), I analyzed the current consumption during different operating modes. At maximum current draw, the device consumes an average of roughly 60mA (see image 1 below), and in deep sleep mode, it consumes about 11µA (see image 2 below). Considering that a typical AA battery has a rough capacity of around 2,200mAh at these draw rates, this translates to approximately 70 hours of operation at maximum current draw and up to 400,000 hours (or about 45 years) in sleep mode.


Current consumption during active mode:
<img src="./assets/media/power/active-large.png" class="img-full" />

Current consumption during sleep mode:
<img src="./assets/media/power/sleep-large.png" class="img-full" />

## Software

> See the Github repo for specifics [zbauman3/digi-roll](https://github.com/zbauman3/digi-roll).

Coming Soon...