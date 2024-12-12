# Potentiometer to LED Array (VHDL and Avalon)

## Building the VHDL and Avalon
To build the "potent_array.vhd" file you will need to create a .vhd file within quartus, or questa if prefered, and then write the code needed. Alongside the potent_array file, you also need to write an "potent_array_avalon.vhd" file that registers will use to interface with the hps bus. Once you have defined both codes, you will head into Platform Designer and instantiate the components and any intercinnects that are needed. After finishing, you will generate the .rbf file that will be used in software to interface with your project.

## Memory Map 

| Offset | Name         | R/W | Purpose                    |
|--------|--------------|-----|----------------------------|
| 0x0    | step_size    | W   | Chose size of pot. step    |
| 0x1    | potent_value | R   | Reads in potentiometer vals|
