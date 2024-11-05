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
#define GPIO_LED0 4
#define GPIO_LED1 5
#define GPIO_LED2 6
#define GPIO_LED3 7
#define GPIO_LED4 8

void config_timer()
{
	*TIMG_T0CONFIG_REG &= ~(0b1 << 9); // use APB_CLK
	*TIMG_T0CONFIG_REG |= (0b1 << 30); // increase
	//*TIMG_T0CONFIG_REG |= (0b1 << 29); // autoreload
	*TIMG_T0LOADLO_REG = 0U;

	*TIMG_T0CONFIG_REG &= ~(0xFFFF << 13); // divider set to 65536
	*TIMG_T0CONFIG_REG |= (8000 << 13);	   // set divider to 800 -> 0,1ms

	*TIMG_T0LOAD_REG = 1U; // Reload start value

	*TIMG_T0ALARMLO_REG = 2500U;	   // Set Alarm Counter to 2000 -> 200ms
	*TIMG_T0LOADHI_REG &= ~0x3FFFF;	   // zero reg
	*TIMG_T0CONFIG_REG |= (0b1 << 31); // Enable Timer
}

void turn_off_all_led()
{
	volatile uint32_t *gpio_out_reg = (volatile uint32_t *)GPIO_OUT_REG;
	*gpio_out_reg &= 0b0000000;
}

void app_main(void)
{
	volatile uint32_t *gpio_out_reg = (volatile uint32_t *)GPIO_OUT_REG;
	volatile uint32_t *gpio_enable_reg = (volatile uint32_t *)GPIO_ENABLE_REG;
	// volatile uint32_t *gpio_in_reg = (volatile uint32_t *)GPIO_IN_REG;
	// volatile uint32_t *timg_t0config_reg = (volatile uint32_t *)TIMG_T0CONFIG_REG;

	config_timer();

	*gpio_enable_reg |= 0b1111; // enable GPIO4-8
	*gpio_out_reg |= 0b1111;
	*TIMG_T0CONFIG_REG |= (0b1 << 10);
	int alarm = (*TIMG_T0CONFIG_REG & (0b1 << 10)) >> 10; // enable alarm
	printf("ALARM %d\n", alarm);

	

	while (1)
	{
		for (int i = 0; i < 3; i++)
		{
			turn_off_all_led();
			*gpio_out_reg |= (1 << i);
			while ((*TIMG_T0CONFIG_REG & (0b1 << 10)) >> 10);
			*TIMG_T0CONFIG_REG |= (0b1 << 10);

		}

		for (int i = 3; i > 0; i--)
		{
			turn_off_all_led();
			*gpio_out_reg |= (1 << i);
			while ((*TIMG_T0CONFIG_REG & (0b1 << 10)) >> 10);
			*TIMG_T0CONFIG_REG |= (0b1 << 10);
		}
	}
}
