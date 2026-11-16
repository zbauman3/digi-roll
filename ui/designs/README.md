# Designs

Bitmap designs as C headers (`.h`) in the editor's format. Load them in the editor (`index.html`),
gallery (`gallery.html`) or animator (`animate.html`), or `#include` them from firmware.

One folder per die (`d4`, `d6`, `d8`, `d10`, `d12`, `d20`, `d100`):

- `<die>/<die>_3d_4bit_lines.h`: static 16×16 4-bit 3D icon with the number.
- `<die>/Animation/<die>_roll_00.h` … `_19.h`: 20-frame rolling loop (no number). Frame 19 leads back into frame 00.
