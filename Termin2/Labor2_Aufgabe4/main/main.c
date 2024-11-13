/*
 * RISV-V implementation for blinking LED
 */

#include <stdio.h>
#include <stdint.h>
#include "esp32c3_reg.h"
#include "7segment.c"
#include "adc.c"
#include <inttypes.h>
#include <esp_intr_alloc.h>

int globalTemp;

void delay(int i) // delay function
{
	for (int j = 0; j < i; j++)
	{
	}
}

int calculateTemp(int adcVal)
{
	int temp = (adcVal - 730) / 14.68; // 0 C at 730, 14,68 per 1 C
	return temp;
}

void interrruptHandler()
{
	globalTemp = calculateTemp((int)adc_read());
	*TIMG_INT_CLR_TIMERS_REG |= 0b1; 
	*TIMG_T0CONFIG_REG |= (0b1 << 10); // set alarm

}

void config_timer()
{
	*TIMG_T0CONFIG_REG &= ~(0b1 << 9); // use APB_CLK
	*TIMG_T0CONFIG_REG |= (0b1 << 30); // increase
	//*TIMG_T0CONFIG_REG |= (0b1 << 29); // autoreload
	*TIMG_T0LOADLO_REG = 0U;

	*TIMG_T0CONFIG_REG &= ~(0xFFFF << 13); // divider set to 65536
	*TIMG_T0CONFIG_REG |= (8000 << 13);	   // set divider to 800 -> 0,1ms

	*TIMG_T0LOAD_REG = 1U; // Reload start value

	*TIMG_T0ALARMLO_REG = 10000U;	   // Set Alarm Counter to 5000
	*TIMG_T0LOADHI_REG &= ~0x3FFFF;	   // zero reg
	*TIMG_T0CONFIG_REG |= (0b1 << 31); // Enable Timer

	*TIMG_INT_ENA_TIMERS_REG |= 0b1; // interrupt enable
	*TIMG_T0CONFIG_REG |= (0b1 << 10); // set alarm
}

void app_main(void)
{
	config_timer();
	initDisplay(); // initialize 7-segment display
	initADC();
	adc_calibrate(1);

	esp_intr_alloc(32, 0, interrruptHandler, NULL, NULL);


	while (1)
	{
		clearPin(9);										  // clear pin 9
		setPin(10);											  // set pin 10
		sevenSegWrite((globalTemp - (globalTemp % 10)) / 10); // display first digit
		for (int m = 0; m < 1000; m++)
		{
		} // delay between switching digits

		clearPin(10);					// clear pin 10
		setPin(9);						// set pin 9
		sevenSegWrite(globalTemp % 10); // display last digit
		for (int n = 0; n < 1000; n++)
		{
		} // delay between switching digits

	}
}