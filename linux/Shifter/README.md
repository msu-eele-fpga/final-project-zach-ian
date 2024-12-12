# Shifter device driver
This file (once compiled) tells linux what to do with the device in the device tree. It defines register offsets and sets up the ability to change the register values by just writing to files within user-space.

## Compile instructions:
use `make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-` to compile, ensure the `KDIR` is set to the correct path for your `linux-socfpga` repo.