/*
 * RISV-V implementation for blinking LED 
 */

#include <stdio.h>
#include <stdint.h>

// Hard definitions of GPIO addresses
#define GPIO_OUT_REG   				0x60004004 
#define GPIO_ENABLE_REG     		0x60004020
#define GPIO_LED 2

void app_main(void)
{
    volatile uint32_t* gpio_out_reg = (volatile uint32_t*) GPIO_OUT_REG;
    volatile uint32_t* gpio_enable_reg = (volatile uint32_t*) GPIO_ENABLE_REG;
	
    *gpio_enable_reg |= 0b0100;  // enable GPIO2 output for LED
	int led = 0;
	
	while(1) {
		if (led == 0) {
			*gpio_out_reg |= (1 << GPIO_LED); 	// Set the level for GPIO2 to HIGH
			led = 1;
			printf("LED on, state %d\n",led);
			}
		else {
        	*gpio_out_reg &= ~(1 << GPIO_LED);	// Set the level for GPIO2 to LOW
			led = 0;
			printf("LED off, state %d\n",led);
			}
		for (int i = 0; i < 10000000; i++) {}	
	}
}

