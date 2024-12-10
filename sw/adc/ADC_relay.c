#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>

int main(void){
	while(1){
		FILE* f1 = fopen ("/sys/devices/platform/ff200000.adc/ch0_raw", "r");
		FILE* f2 = fopen ("/sys/devices/platform/ff200000.adc/ch1_raw", "r");
		FILE* f3 = fopen ("/sys/devices/platform/ff200000.adc/ch2_raw", "r");
		FILE* f4 = fopen ("/sys/devices/platform/ff200020.rgbled/red_duty", "w");
		FILE* f5 = fopen ("/sys/devices/platform/ff200020.rgbled/green_duty", "w");
		FILE* f6 = fopen ("/sys/devices/platform/ff200020.rgbled/blue_duty", "w");
		int a = 0;
		int b = 0;
		int c = 0;
		fscanf(f1, "%d", &a);
		fscanf(f2, "%d", &b);
		fscanf(f3, "%d", &c);
		a = a >> 1;
		b = b >> 1;
		c = c >> 1;
		//printf("%d %d %d\n", a, b, c);
		
		fprintf(f4, "%d",a);
		fprintf(f5, "%d",b);
		fprintf(f6, "%d",c);

		fclose(f1);
		fclose(f2);
		fclose(f3);
		fclose(f4);
		fclose(f5);
		fclose(f6);
	}
}
