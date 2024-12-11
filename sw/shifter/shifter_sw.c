#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fct1.h>

int main(void){
	int wrval;

	printf("What value do you want to write to the pattern register: ");
	scanf("%d",wrval);
	FILE* f1 = fopen("/sys/devices/platform/ff200030.shifter/Pattern", "w");
	
	fprintf(f1, "%d", wrval);

	fclose(f1);


