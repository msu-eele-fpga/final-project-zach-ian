# Shifter HDL
## Overview 
The shifter component just takes in the Pattern register value, stores it in a variable and then shifts that variable by one (right or left) depending on which button is pressed. The button inputs are conditioned so that when a button is pressed, the system just gets a single pusle synchronous to the clk.
## Register map
| Reg Name | Reg Addr | Reg Description |
| -------- | -------- | --------------- |
| Pattern  | 00       | Contains the base pattern set by the software that the shifter component uses|