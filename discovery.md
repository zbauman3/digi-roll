# DigiRoll V2 — Discovery

Working doc for V2 discovery. It is meant for both humans and agents.

**Status legend**

- **Decided**: committed. Don't relitigate it without the owner.
- **Tentative**: the current leaning, still open to change.
- **Open**: not researched yet. Don't fill these with assumptions; the owner works through them one at a time and asks for help when ready.

> Agents: V2 is a from-scratch redesign. V1 (the `main` branch) is reference only. Don't carry V1 parts, pins or architecture into V2 unless this doc says so.

---

## 1. Decided

- **Clean start.** No V1 code or hardware is carried forward.
- **Commercial product.** V2 is designed to be manufactured and sold.
- **Repo split / licensing:**
  - Firmware: open source, in this repo (`digi-roll`).
  - Hardware (schematics, PCB, enclosure): kept in a **separate private repo**. Don't add hardware design files to this repo.
  - The license that replaces V1's MIT-everything is still TBD (see Open).
- **The spinner is the main input.** It selects dice (by scrolling through options) **and** rolls.
- **MCU: RP2350.** Build the final system from the RP2350 reference design, kept as minimal as possible. Development starts on an RP2350 dev board. The RP2040 and the AVR EA alternative are dropped.
- **Power on/off: soft latch through the regulator's enable (EN) pin.** This is the simple version; the MOSFET high-side soft latch was considered and not chosen.
  - A momentary button pulls EN high. The RP2350 boots and holds EN high with a GPIO.
  - To shut down, the MCU releases the GPIO and a pull-down (~1 MΩ) pulls EN low, cutting power to everything after the regulator.
  - The button and the GPIO likely need a diode each (diode-OR) so they don't fight.
  - Scope: the latch only switches the system power rail, by controlling whether the regulator is enabled. It has no other job.
  - **To resolve:** the button side drives EN from the battery, up to ~8.4 V. Two consequences:
    - The chosen regulator's EN pin must be rated up to max VIN, or the button path needs a resistor divider or clamp.
    - The diode on the GPIO side is **required**, because it keeps battery voltage off the RP2350's 3.3 V pin.
- **Wake source: button only.** The spinner does not need to wake the device.
- **Display: 16×16 LED matrix, monochrome (each pixel on or off).**
  - Rejected: 16×8 (one line of four characters, little gain over V1's 7-segment), 32×8 (the device should be roughly square), and a Sharp memory LCD (more than wanted; the glowing LED look stays).
  - Content: a 3×5 font gives two lines of four characters. Dice selection (e.g. `3d20`) on top, result below, spare rows for a progress bar or roll animation.
  - **Icons and animations are a V2 goal**: 16×16 single-color pixel art, such as fire (e.g. a flicker on a crit), a skull and crossbones, or a d20 outline.
  - **Double buffering is required**: draw into one buffer while the other is scanned out, then swap. Memory is trivial (a 1-bit frame is 32 bytes; the RP2350 has 520 KB of RAM).
  - **Driven by RP2350 PIO + DMA**: DMA streams the frame buffer to PIO like a custom serial bus, and firmware only writes the buffer. Each PIO block holds 32 instructions shared by 4 state machines (3 blocks total); a row-scan loop fits easily.
- **Batteries: 2× AA-size cells, safe with any AA-size cell a customer might insert.** That covers alkaline, NiMH rechargeable, lithium primary, 1.5 V-regulated USB lithium AAs, and 3.7 V Li-ion 14500s.
  - Note: NiMH rechargeables are ~1.2 V nominal, *lower* than alkaline (1.5 V). The high-voltage case is Li-ion 14500s (~3.7 V nominal, 4.2 V full).
  - **Mixed cells are unsupported.** The design assumes both cells are the same type, and the user docs will tell customers not to mix them. Mixed cells still have to fall within the supported input voltage range so they don't damage anything. Protecting against over-discharge with mixed cells is not a design goal.

## 2. Tentative

| Area            | Current leaning                                                                                          | Notes                                                                                                                              |
| --------------- | -------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------- |
| Spinner sensing | **Magnetic angle sensor** (magnetic rotary encoder, e.g. AS5600-class: 12-bit, 4096 steps/turn). Diametrically magnetized magnet on the axle end, chip directly under it | Replaces V1's on/off Hall switch. Reads absolute angle, so it never loses position. Gives fine position for scrolling plus spin detection for rolling |
| Spinner form    | A **wheel** coming out of the **right side** of the case, with ~2/3 inside and only a small part showing |                                                                                                                                    |
| Spinner detents | **DIY magnetic detents**: a ring of magnets in the wheel rim plus a fixed magnet or steel piece (not mechanical) | Feel only, not part of sensing. Magnets add useful weight/momentum to the 3D-printed wheel; extra weights may be added. Keep detent magnets on the outer rim, away from the center sensor |
| Spinner mapping | Divide the 4096 steps evenly by the detent count (e.g. 12). Align detent rest positions to the **center** of each section, not the edges | Firmware calibration offset so a resting wheel doesn't jitter between two values |
| Buttons         | Minimal, **likely 2** (select / cancel)                                                                  | Still being thought through                                                                                                        |
| Form factor     | **Very undecided; needs a lot more discovery.** Current ideas: LED matrix fills most of the top, 2 buttons to one side, wheel on the right side. "Roughly square" and an earlier **8" W × 5" H × 2–3" D** have both come up | Don't treat any dimension as a requirement yet |
| Build plan      | Hand-build the first prototype, then use a manufacturer for production PCBs | |

## 3. Open — to research / discuss

Rough grouping, not a priority order.

### Display

- [x] ~~Matrix size~~ 16×16 (Decided 2026-11-15)
- [ ] Physical pitch for the target case size
- [ ] LED choice: color, low-current part, brightness at low drive current
- [x] ~~Drive method~~ RP2350 PIO + DMA multiplexing (Decided 2026-11-15)
- [ ] Refresh rate and duty cycle versus power budget
- [ ] What the UI shows: dice selection, roll animation, results, sum, and anything else (baseline layout and icon goal are Decided; details open)
- [ ] Grayscale (nice-to-have, not a requirement): a few brightness levels (e.g. low/medium/high) via bit-plane / frame-rate modulation, layered on top of the 1-bit core driver. Four levels = 2 bits/pixel = 64 bytes/frame. Main cost is refresh rate: scan fast enough that dim pixels don't flicker. PIO at up to 150 MHz should handle it easily

### Spinner / input

- [ ] Choose the magnetic angle sensor part: resolution, interface, power, sleep/wake behavior
- [ ] Wheel size: roughly 3–4 in diameter under consideration, **not decided**
- [ ] Mechanics: bearings, magnet mounting, detent count
- [ ] Test early that the rim detent magnets don't disturb the angle sensor
- [ ] Interaction model: how scroll vs. roll is told apart (speed? a button? a threshold?)
- [ ] Randomness source and seeding (V1 seeded from spin timing)
- [x] ~~Wake-from-sleep: can the spinner wake the device, or only buttons?~~ Button only (Decided 2026-11-15)
- [ ] Final button count and what each one does

### Power

- [ ] Input voltage range: roughly 1.6 V (two depleted NiMH) to ~8.4 V (two fully charged 14500s). Wider than many small buck-boost chips allow
- [ ] Regulator: one wide-input buck-boost should cover the whole range on its own (no separate pre-regulator). Needs an EN pin (for the soft latch), low shutdown current, and good efficiency. TI TPS63070 is a possible candidate, **not yet verified** against its datasheet. **Research needed:** its input range is believed to be ~2.0–16 V, which covers the top end but not the 1.6 V floor. Either accept a ~2.0 V cutoff (losing some NiMH/alkaline capacity) or find a part that covers 1.6–8.4 V. Also check that its EN pin is rated to VIN (see the soft latch in Decided)
- [ ] Reverse-polarity protection: assume a cell goes in backwards. Prefer a MOSFET-based circuit over a series diode to avoid the voltage drop at low battery
- [ ] Over-discharge / undervoltage cutoff: unprotected Li-ion cells are damaged by deep discharge, so the device needs a low-battery shutdown. The right cutoff depends on chemistry; options include detecting chemistry from the starting voltage, or relying on a regulator UVLO
- [x] ~~Mixed cells~~ Unsupported; the design assumes matched cells and the user docs warn against mixing (Decided 2026-11-15)
- [ ] Whether charging happens on the device (USB-C?) or the cells are removed to charge
- [ ] Power budget: active current, sleep current, target battery life

### MCU

- [ ] RP2350 sleep current (known risk). The soft latch mostly sidesteps it: when off, the drain is the regulator's shutdown current plus leakage, not the RP2350's dormant current. Still matters for any idle-but-on state
- [ ] Toolchain/framework (Pico SDK, Arduino, Rust, etc.)

### Enclosure / form factor

- [ ] Final dimensions
- [ ] Battery access
- [ ] Materials and manufacturing method for production (3D print vs. injection molding, etc.)

### Business / repo

- [ ] Firmware license (MIT, GPL, etc.) and how it interacts with the closed hardware
- [ ] Interface contract between the firmware repo and the private hardware repo (pin maps, board revisions)
- [ ] Manufacturing: PCBA vendor, BOM cost targets, certification (FCC/CE) if relevant

## 4. V1 reference (the `main` branch)

For context only. These are not V2 decisions.

- ATtiny84A, Arduino + PlatformIO + AceRoutine, about 7 KB of flash
- 8 tactile buttons via a CD4532 priority encoder: reset plus d4, d6, d8, d10, d12, d20 and d%. Pressing again adds dice, up to 9
- D20 spinner on bearings with a magnet. An **on/off Hall switch** (US5881) detects passes, and the spin timing seeds the RNG
- 4-digit 7-segment display (HT16K33 breakout) and 7 status LEDs on a 74HC595
- 2×AA → 5V boost breakout (~88% efficient). About 60 mA active, about 11 µA asleep
- Pain points from the V1 write-up:
  - Breakout boards raised unit cost
  - The boost efficiency was only acceptable
  - The Hall sensor needed GPIO power-gating
  - The HT16K33 needed manual sleep handling
  - Controller logic leaked into the state model
- Write-up: https://www.zanebauman.com/articles/digi-roll

## 5. Decision log

Add an entry each time something moves from Tentative or Open to Decided.

| Date       | Decision                                                                                           | Rationale                    |
| ---------- | -------------------------------------------------------------------------------------------------- | ---------------------------- |
| 2026-11-12 | V2 is a from-scratch commercial product; firmware is open source, hardware lives in a private repo | Plan to manufacture and sell |
| 2026-11-15 | MCU is the RP2350, built from its reference design; RP2040 and AVR EA dropped | Already have a dev board; want a very minimal design |
| 2026-11-15 | Power on/off via a soft latch on the regulator EN pin (button + GPIO hold, ~1 MΩ pull-down) | Fully off when idle, so off-current is just regulator shutdown; fewer parts than the MOSFET version. Replaces the supervisor-MCU idea |
| 2026-11-15 | Wake from off is button only; the spinner does not wake the device | Acceptable UX, and required by a fully-off MCU |
| 2026-11-15 | Display is a 16×16 monochrome LED matrix; 16×8, 32×8 and a Sharp memory LCD rejected | Square device; two lines of text plus room for icons, which a 7-segment can't do |
| 2026-11-15 | Icons and animations (16×16 pixel art) are a V2 goal | A more graphical interface than V1 |
| 2026-11-15 | Display uses double buffering | Prevents tearing in animations; costs almost no memory |
| 2026-11-15 | Display driven by RP2350 PIO + DMA from a frame buffer | Keeps the CPU free; firmware only writes the buffer |
| 2026-11-15 | Runs on 2× AA-size cells and must be safe with any AA-size chemistry, including 3.7 V Li-ion 14500s | Consumer product; customers will insert whatever fits |
| 2026-11-15 | Mixed cell types are unsupported; design for matched cells and warn in the user docs | Keeps cutoff logic simple; the input range still has to tolerate mixed cells safely |
