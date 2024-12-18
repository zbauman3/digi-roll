---
layout: default
title: Instructions
---

## Assembly Notes

### 7-segment Backpack
- Cut off all leads from the LEDs to the backpack after soldering - they must be cut shorter than the IC on the back of the board.
- Carefully break off the STEMMA QT connectors, they're not needed and use extra space.
- Remove the plastic spacers from the `+`, `-`, `SDA`, and `SCL` headers after soldering.
- When adding the backpack to the main board, make sure that the backpack is resting flat on the IC on the back.
- Use hot glue to hold it in place at the correct angle. Make sure alignment is straight before soldering.
- On the back of the backpack is an LED. There's a jumper labeled `LED` that you should cut to disable it and save power.

### Power Boost
- Take off the plastic spacers from the headers after soldering.

### HALL effect
- This should be placed on the bottom side of the board, instead of the top.
- It should face so that the angled side points towards the spinner.
- Solder with the sensor leaning at an angle towards the spinner if possible.

### ISP Headers
- These should be placed on the bottom side of the board, so that you can flash new software by sliding open the case slightly.

### Battery
- The connector between the battery and the board should be 12-13cm.

### Spinner
- Don't tighten the screw too much or else it won't spin
- It will loosen over time – use it for a while before gluing.

### 3D Printing
- Making the buttons transparent is trial and error. Using semi-translucent filament and only a few top/bottom layers helps.
- Tolerances for thew sliding mechanism are tight. You might need to adjust them depending on your printer's abilities.
- The back battery door should be printed on its long side, so that the direction of stress on the clip doesn't separate layers.

### Misc
- All components should be as tight/close to the board as possible.
- Take note of the direction of the MCU/ICs on the PCB, they're not all the same.
- Don't tighten the main external screws too much, this bends the case from torque.
- Use high-temp hot glue when gluing parts.
- **There are no protection diodes between the ISP headers and the battery**. The battery should be off when using the ISP programmer as a power source.