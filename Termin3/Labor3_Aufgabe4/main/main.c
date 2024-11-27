/*
 * RISV-V implementation for blinking LED
 */

#include <stdio.h>
#include <stdint.h>
#include "esp32c3_reg.h"
#include "timer.c"
#include "adc.c"
#include "sinus255.h"
#include <inttypes.h>
#include <esp_intr_alloc.h>


int global_output;

void setPin(int pin)
{
	*GPIO_OUT_REG = pin << 1;
}

void ISR(){

	setPin(sinus[global_output]);
	if(global_output >= 99) global_output = 0;
	else global_output++;
	//reactivate timer
	*TIMG_INT_CLR_TIMERS_REG |= 0b1; 
	*TIMG_T0CONFIG_REG |= (0b1 << 10); // set alarm
}

void setup()
{
	*GPIO_ENABLE_REG |= 0b111111110; // enable output of GPIO pins 1..8
	*IO_MUX_GPIO4_REG |= (1 << 12); // set IO Mux function 1 (see Table 5.3 of Techn. Ref. Manual)
	*IO_MUX_GPIO5_REG |= (1 << 12); // set IO Mux function 1 (see Table 5.3 of Techn. Ref. Manual)
	*IO_MUX_GPIO6_REG |= (1 << 12); // set IO Mux function 1 (see Table 5.3 of Techn. Ref. Manual)
	*IO_MUX_GPIO7_REG |= (1 << 12); // set IO Mux function 1 (see Table 5.3 of Techn. Ref. Manual)
}


void app_main(void)
{
	initADC();
	adc_calibrate(1);
	setup();
	setup_timer();

	esp_intr_alloc(32, 0, ISR, NULL, NULL);



	while (1)
	{
		set_timer((int)adc_read());
	}
}