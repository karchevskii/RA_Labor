/*
 * RISV-V implementation for blinking LED
 */

#include <stdio.h>
#include <stdint.h>
#include "esp32c3_reg.h"

// Hard definitions of GPIO addresses
#define GPIO_OUT_REG 0x60004004
#define GPIO_IN_REG 0x6000403C
#define GPIO_ENABLE_REG 0x60004020
#define GPIO_LED 2

void config_timer()
{
	*TIMG_T0CONFIG_REG &= ~(0b1 << 9); // use APB_CLK
	*TIMG_T0CONFIG_REG |= (0b1 << 30); // increase
	//*TIMG_T0CONFIG_REG |= (0b1 << 29); // autoreload
	*TIMG_T0LOADLO_REG = 0U;

	*TIMG_T0CONFIG_REG &= ~(0xFFFF << 13); // divider set to 65536
	*TIMG_T0CONFIG_REG |= (8000 << 13);	   // set divider to 800 -> 0,1ms

	*TIMG_T0LOAD_REG = 1U; // Reload start value

	*TIMG_T0ALARMLO_REG = 5000U;	   // Set Alarm Counter to 5000
	*TIMG_T0LOADHI_REG &= ~0x3FFFF;	   // zero reg
	*TIMG_T0CONFIG_REG |= (0b1 << 31); // Enable Timer
}

void app_main(void)
{
	volatile uint32_t *gpio_out_reg = (volatile uint32_t *)GPIO_OUT_REG;
	volatile uint32_t *gpio_enable_reg = (volatile uint32_t *)GPIO_ENABLE_REG;
	volatile uint32_t *gpio_in_reg = (volatile uint32_t *)GPIO_IN_REG;
	volatile uint32_t *timg_t0config_reg = (volatile uint32_t *)TIMG_T0CONFIG_REG;

	config_timer();

	*gpio_enable_reg |= 0b10000100;	   // enable GPIO2 and GPIO7 output for LED and Button
	*IO_MUX_GPIO7_REG &= 0b1000000000; // Set FUN_IE to HIGH
	int led = 0;
	int blinken = 0;
	int alarm = (*TIMG_T0CONFIG_REG & (0b1 << 10)) >> 10;
	printf("ALARM %d\n", alarm);

	while (1)
	{
		int button_status = (*gpio_in_reg & 0b10000000) >> 7;
		printf("Button state %d\n", (!button_status));
		if (!button_status)
		{
			if (blinken == 0)
			{

				blinken = 1;
				*TIMG_T0CONFIG_REG |= (0b1 << 10); // set alarm
				while (!((*gpio_in_reg & 0b10000000) >> 7));
				while (((*gpio_in_reg & 0b10000000) >> 7))
				{
					if (!((*TIMG_T0CONFIG_REG & (0b1 << 10)) >> 10))
					{
						alarm = (*TIMG_T0CONFIG_REG & (0b1 << 10)) >> 10;
						if (led == 0)
						{
							*gpio_out_reg |= (1 << GPIO_LED); // Set the level for GPIO2 to HIGH
							led = 1;
							printf("LED on, state %d\n", led);
						}
						else
						{
							*gpio_out_reg &= ~(1 << GPIO_LED); // Set the level for GPIO2 to LOW
							led = 0;
							printf("LED off, state %d\n", led);
						}
						*TIMG_T0CONFIG_REG |= (0b1 << 10); // set alarm
					}
				}
				blinken = 0;
				// *gpio_out_reg &= ~(1 << GPIO_LED); // Set the level for GPIO2 to LOW
				// led = 0;
				// printf("LED off, state %d\n", led);
			}
			while (!((*gpio_in_reg & 0b10000000) >> 7));
		}
	}
}
