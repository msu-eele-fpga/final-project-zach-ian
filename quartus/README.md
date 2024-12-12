# Quartus Directory Overview

## Purpose
The quartus directory includes the top file for the VHDL project which has important aspects including the `de10nano_top.vhd` used for full system instantiation, our `soc_system.rbf` which is used in our kernel for component identification, relevant `.tcl` files that describes components on the hps bus, and finally our `.qip` file which contains our IP cores.

## de10nano.vhd
Our de10nano file required these minor changes to properly instantiate components.


### Port Definitions:
```
258:  shifter_avalon_output	      : out   std_logic_vector(7 downto 0);
259:  shifter_avalon_pb_right	  : in    std_logic;
260:  shifter_avalon_pb_left	  : in    std_logic;
261:  potent_array_led_value      : out  std_logic_vector(7 downto 0)
```
### Interconnects
PWM LED control:
```
364:  pwm_avalon_blue_pwm_signal => gpio_0(0),
365:  pwm_avalon_green_pwm_signal => gpio_0(1),
366:  pwm_avalon_red_pwm_signal => gpio_0(2),
```

Shifter control:
```
369:  std_ulogic_vector(shifter_avalon_output) => gpio_0(17 downto 10),
370:  shifter_avalon_pb_left => gpio_0(33),
371:  shifter_avalon_pb_right => gpio_0(35),
```

Potentiometer LED Array:
``` 
374:  std_logic_vector(potent_array_led_value) => led
```

## soc_system.rbf

The `soc_system.rbf` file as used in our kernel to configure the FPGA fabric within the SoC and set enviorment variables.

## .tcl Files

`.tcl` files are used so platform designer can properly place components and include appropriate attributes.

## .qip files

A `.qip` file, also known as a Quartus Prime IP File, is a single file that contains paths to all files for an IP core.