# Device tree

device tree source file.


## Overview
Main body of the file:
```
de10nano_adc: adc@ff200000 {
		compatible = "adsd,de10nano_adc";
		reg = <0xff200000 32>;
	};
	rgbled: rgbled@ff200020 {
		compatible = "Ian,rgbled";
		reg = <0xff200020 16>;
	};
	shifter: shifter@ff200030 {
		compatible = "Ian,shifter";
		reg = <0xff200030 16>;
	};
	potent_array: potent_array@ff200040 {
		compatible = "Zach,potent_array";
		reg = <0xff200040 16>;
	};
```
This tells the linux device tree that the adc component is at base address `0xff200000`, rgbled is at `0xff200020`, shifter is at `0x200030`, and potent array is at `0xff200040`.
