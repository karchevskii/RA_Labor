/*
 * RISV-V implementation for blinking LED
 */

#include <stdio.h>
#include <stdint.h>
#include "esp32c3_reg.h"
#include <inttypes.h>


void setup()
{
	*GPIO_ENABLE_REG |= 0b11111111; // enable output of GPIO pins 1..8
	*IO_MUX_GPIO4_REG |= (1 << 12); // set IO Mux function 1 (see Table 5.3 of Techn. Ref. Manual)
	*IO_MUX_GPIO5_REG |= (1 << 12); // set IO Mux function 1 (see Table 5.3 of Techn. Ref. Manual)
	*IO_MUX_GPIO6_REG |= (1 << 12); // set IO Mux function 1 (see Table 5.3 of Techn. Ref. Manual)
	*IO_MUX_GPIO7_REG |= (1 << 12); // set IO Mux function 1 (see Table 5.3 of Techn. Ref. Manual)
									//    *GPIO_OUT_REG |= 0b11111111;
}


void setPin(int pin)
{
	*GPIO_OUT_REG = pin ;//& 0xFF;
}

void app_main(void)
{
	setup();

	while (1)
	{

		for (int i = 0; i < 511; i++)
		{
			setPin(i);

			for (int j = 0; j < 600; j++)
			{
				/* code */
			}
		}
	}
}