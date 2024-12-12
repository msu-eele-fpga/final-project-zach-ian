# Potentiometer LED Array Software
The `potent_array.c` file includes all necessary software to read and write values to the register though either linux or our Putty terminal accessing linux.

## Code:

### Integers to Store incoming/Outgoing Values:
```
int wrval;
int a;
```

### Output/Input for the User:
```
printf("What value do you want to write to the step size register: (1 or 0)");  
scanf("%d",&wrval); 
```

### Register Access:
```
FILE* f2 = fopen("/sys/devices/platform/ff200040.potent_array/potent", "w");
FILE* f3 = fopen("/sys/devices/platform/ff200000.adc/ch0_raw", "r");
```
