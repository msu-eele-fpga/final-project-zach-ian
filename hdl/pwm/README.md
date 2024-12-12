# Shifter HDL
## Overview 
The pwm component takes in a duty cycle and a period and outputs the appropriate signal. The pwm_controller component is instantiated three times in the avalon file for each Red Green Blue channels.
## Register map
| Reg Name | Reg Addr | Reg Description |
| -------- | -------- | --------------- |
| duty_cycle_r  | 00  | desired red channel duty cycle |
| duty_cycle_g  | 01  | desired green channel duty cycle |
| duty_cycle_b  | 10  | desired blue channel duty cycle |
| pwm_period    | 11  | desired period for all channels |