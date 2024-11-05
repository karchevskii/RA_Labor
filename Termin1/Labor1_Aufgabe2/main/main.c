/*
 * RISV-V implementation for blinking LED 
 */

#include <stdio.h>
#include <stdint.h>
#include "esp32c3_reg.h"

// Hard definitions of GPIO addresses
#define GPIO_OUT_REG   				0x60004004 
#define GPIO_IN_REG					0x6000403C
#define GPIO_ENABLE_REG     		0x60004020
#define GPIO_LED 2

void app_main(void)
{
    volatile uint32_t* gpio_out_reg = (volatile uint32_t*) GPIO_OUT_REG;
    volatile uint32_t* gpio_enable_reg = (volatile uint32_t*) GPIO_ENABLE_REG;
	volatile uint32_t* gpio_in_reg = (volatile uint32_t*) GPIO_IN_REG;
	
    *gpio_enable_reg |= 0b10000100;  // enable GPIO2 and GPIO7 output for LED and Button
	*IO_MUX_GPIO7_REG &= 0b1000000000; // Set FUN_IE to HIGH
	int led = 0;
	while(1) {
		int button_status = (*gpio_in_reg & 0b10000000 ) >> 7;
		printf("Button state %d\n", (!button_status));
		if(!button_status){
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
			while(!((*gpio_in_reg & 0b10000000 ) >> 7));
		}
	}
}
