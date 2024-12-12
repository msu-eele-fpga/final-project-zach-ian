# ADC -> RGB_LED software code
This program loops infinity opening the adc channel files of interest, and the rgbled duty cycle files, reads from each adc channel, divides that value by 2 (because the max value for the duty cycle is 2047 and the max adc value is 4095), then writes each result to the respective duty cycle file.
