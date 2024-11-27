/*
 * RISV-V implementation for blinking LED
 */

#include <stdio.h>
#include <stdint.h>
#include "esp32c3_reg.h"
#include "adc.c"
#include "pwm.c"
#include <inttypes.h>


int calcPercent(int adcVal) {

	// smallest adcVal is 30, biggest is 4095
	int percentage = 5 + ((adcVal - 30)*90 /4066);
	return percentage;
}


void app_main(void)
{
	//init ADC, GPIO Pin 0
	initADC();
	adc_calibrate(1);

	// init PWM, GPIO Pin 2
	setup_pwm();
	set_pwm_duty(0);


	while (1)
	{

		int percentage = calcPercent((int)adc_read());

		set_pwm_duty(percentage);


		for (int i = 0; i < 10000000; i++)
		{
			/* code */
		}
		
	}
}