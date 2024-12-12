# 467-final-project
## Overview
This project uses hardware and software in tandem to implement various functionalities described below:  
1. RGB LED controlled by PWM based on the position of 3 potentiometers 
	* RGB ELD
	* ADC
	* Potentiometers
2. LED array with pattern that shifts dependent on push buttons
	* 8 external LEDs
	* One register to hold the base pattern
	* 2 push buttons to determine shift right or shift left.
3. LED array that displays relative position of potentiometer  
	* 8 on board LEDs
	* Two registers  
		1. Sending the potentiometer position
		2. Chooses what pattern is displayed
	* One potentiometer.
## Directories
| Dir name | Description | 
| -------- | ----------- | 
| docs     | Proposal file and submission guidelines |
| hdl      | Where the VHDL code files go for each of the three components |
| linux    | Where device drivers for each component go and where the modifications to the linux device tree are shown |
| quartus  | Where the quartus project files lie to compile the hardware devices into one design (Noteworthy files: [top file](quartus/de10nano_top.vhd), Tickle (tcl) files) |
| sw       | Where C files go for executing user-space code that connects our hardware components and drives them with values. |
| utils    | Something (helpful files for our reference)
## 