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

	asm volatile (
		"la t0, sinus			\n\t"	// load address of sinus
		"addi t1, zero, 99		\n\t"	// load  99 into t1
		"slli t3, %0, 2			\n\t"	// shift global_output << 2 
		"sll t5, t0, t3			\n\t"	// 

		"lw t2, t0				\n\t"	// load A[i]
		"addi t0, t0, 1         \n\t"   // A++
		
		"lw t4, GPIO_OUT_REG	\n\t"	// load GPIO_OUT_REG addr
		"sw t2, 1(t4)			\n\t"	// store word t2 at t4

		"bge %0, t1, 2f         \n\t"   // branch to 2, if global_output is 99 or greater
		"addi %0, %0, 1			\n\t"	// global_output ++
		"beq zero, zero, 3f		\n\t"	// jump to end
		"2:                         "	
		"addi %0, zero, 0  		\n\t"	// set global_output to 0

		"3:"	
	: "+r" (global_output)
	: "m" (GPIO_OUT_REG), "m" (sinus)
	: "t0", "t1", "t2", "t3", "t4", "t5"
	);

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