/*
 * RISV-V implementation for blinking LED
 */

#include <stdio.h>
#include <stdint.h>
#include "esp32c3_reg.h"

void setup_pwm()
{

	// Configure GPIOx as PWM output
	*GPIO_ENABLE_REG |= (1 << 2);
	*GPIO_FUNC2_OUT_SEL_CFG_REG = 45;
	*IO_MUX_GPIO2_REG |= (1 << 12);

	// Enable PWM Clock
	*SYSTEM_PERIP_CLK_EN0_REG |= (0b1 << 11);
	// Reset PWM
	*SYSTEM_PERIP_RST_EN0_REG |= (0b1 << 11);
	*SYSTEM_PERIP_RST_EN0_REG &= ~(0b1 << 11);

	*LEDC_CONF_REG = 1;

	// Configure PWM Timer0 for 1 MHz with duty resolution for 1 kHz PWM
	*LEDC_TIMER0_CONF_REG = (80 << 12); // Set frequency divider
	*LEDC_TIMER0_CONF_REG |= (10);		// Set duty resolution to 1024 (0-1023)
	*LEDC_TIMER0_CONF_REG |= 1 << 25;

	// Configure PWM Channel 0
	*LEDC_CH0_CONF0_REG = 0; // Set hpoint to 0
	*LEDC_CH0_CONF0_REG |= 1 << 2;
	*LEDC_CH0_CONF0_REG |= (1 << 4); // Enable PWM channel 0
}

void set_pwm_duty(int ratio)
{
	*LEDC_CH0_CONF1_REG |= 1 << 31;
	*LEDC_CH0_HPOINT_REG = 0;
	*LEDC_CH0_DUTY_REG = ((ratio * 1023) / 100) << 4;
	*LEDC_CH0_CONF0_REG |= 1 << 4;
}

void app_main(void)
{
	*GPIO_ENABLE_REG |= 0b100;

	setup_pwm();
	set_pwm_duty(0);

	while (1)
	{
		for (int i = 0; i <= 1023; i += 10)
		{
			set_pwm_duty(i);
			for (int j = 0; j < 10000; j++)
				;
		}
	set_pwm_duty(0);
		for (int i = 1023; i >= 0; i -= 10)
		{
			printf("countdown");
			set_pwm_duty(i);
			for (int j = 0; j < 10000; j++)
				;
		}
	}
}