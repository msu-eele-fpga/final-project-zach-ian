#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(void){
	int wrval;

	printf("What value do you want to write to the pattern register: ");
	scanf("%d",&wrval);
	FILE* f1 = fopen("/sys/devices/platform/ff200030.shifter/pattern", "w");
	fprintf(f1, "%d", wrval);
	fclose(f1);
	FILE* f2 = fopen("/sys/devices/platform/ff200030.shifter/pattern", "w");
	fprintf(f2, "%d", wrval);
	fclose(f2);
};

