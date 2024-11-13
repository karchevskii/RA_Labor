/*
 * RISV-V implementation for blinking LED
 */

#include <stdio.h>
#include <stdint.h>
//#include "esp32c3_reg.h"
#include "7segment.c"

void app_main(void)
{
	initDisplay(); // initialize the 7-segment display
	while (1)
	{
		for (int i = 0; i < 10; i++) // count from 0 to 9
		{
			sevenSegWrite(i); // write the digit i to the 7-segment display
			for (int j = 0; j < 10000000; j++)  // delay
			{
			} 
		}
	}
}