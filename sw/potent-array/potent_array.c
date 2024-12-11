#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(void){
	int wrval;
    int a;

	printf("What value do you want to write to the step size register: (1 or 0)");
	scanf("%d",&wrval);
	FILE* f1 = fopen("/sys/devices/platform/ff200040.potent_array/step", "w");
	fprintf(f1, "%d", wrval);
	fclose(f1);

    while(1) {
        FILE* f2 = fopen("/sys/devices/platform/ff200040.potent_array/potent", "w");
        FILE* f3 = fopen("/sys/devices/platform/ff200000.adc/ch0_raw", "r");
        fscanf(f3,"%d", &a);
        fprintf(f2,"%d", a);
        fclose(f2);
        fclose(f3);
    }
};

