/*
 * RISV-V implementation for blinking LED
 */

#include <stdio.h>
#include <stdint.h>
#include "esp32c3_reg.h"
#include "7segment.c"
#include "adc.c"
#include <inttypes.h>

void delay(int i) // delay function
{
	for (int j = 0; j < i; j++)
	{
	}
}

int calculateTemp(int adcVal)
{
	int temp = (adcVal - 730) / 14.68; // 0 C at 730, 14,68 per 1 C
	printf("%d\n", temp);
	return temp;
}

void app_main(void)
{
	initDisplay(); // initialize 7-segment display
	initADC();
	adc_calibrate(1);
	while (1)
	{
		// printf("RAW: %d", (int)adc_read());
		int temperature = calculateTemp((int)adc_read());
		// printf("%d", temperature);
		for (int k = 0; k < 10000; k++) // display the two digits for k times
		{
			clearPin(9);	  // clear pin 9
			setPin(10);		  // set pin 10
			sevenSegWrite((temperature - (temperature % 10)) / 10); // display first digit
			for (int m = 0; m < 1000; m++)
			{
			} // delay between switching digits

			clearPin(10);	  // clear pin 10
			setPin(9);		  // set pin 9
			sevenSegWrite(temperature % 10); // display last digit
			for (int n = 0; n < 1000; n++)
			{
			} // delay between switching digits
		}

		// for (int i = 0; i < 10; i++) // count first digit from 0 to 9
		// {
		// 	for (int j = 0; j < 10; j++) // count second digit from 0 to 9
		// 	{
		// 		for (int k = 0; k < 10000; k++) // display the two digits for k times
		// 		{
		// 			clearPin(9);	  // clear pin 9
		// 			setPin(10);		  // set pin 10
		// 			sevenSegWrite(i); // display i
		// 			for (int m = 0; m < 1000; m++)
		// 			{
		// 			} // delay between switching digits

		// 			clearPin(10);	  // clear pin 10
		// 			setPin(9);		  // set pin 9
		// 			sevenSegWrite(j); // display j
		// 			for (int n = 0; n < 1000; n++)
		// 			{
		// 			} // delay between switching digits
		// 		}
		// 	}
		// }
	}
}