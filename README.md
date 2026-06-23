This is a bare metal LED blinking and GPIO toggling on the LPC55S06-EVK. No HAL (SDK) functions are used. Just direct register access.
This is a minimal setup and a starting point for any bare metal projects.

![](/Doc/IMG_6323.jpg)

Evaluation board

![](/Doc/bmp_152_000.bmp)

10 ms system tick

| Output       | MCU Port | Function | Connector | Connector Pin |
|--------------|----------|----------|-----------|---------------|
| 10 ms toggle | P0.9     | GPIO     | J13       | 6             |
| GND          | GND      | GND      | J10       | 17            |

Hardware

LPC55S06-EVK: https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/lpcxpresso-boards/lpcxpresso-development-board-for-lpc55s0x-0x-family-of-mcus:LPC55S06-EVK
