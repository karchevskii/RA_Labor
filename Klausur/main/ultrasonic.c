#include "esp32c3_reg.h"
#include "defines.h"

void setupGPIOsUltrasonic()
{
	// set TRIG as output
	*GPIO_ENABLE_REG |= (1 << TRIG);
	*IO_MUX_GPIO1_REG |= (1 << 9);

	// set ECHO (GPIO1) as input and interrupt
	*IO_MUX_GPIO1_REG |= (1 << 12);	 // set IO_MUX Func1
	*GPIO_PIN1_REG &= ~(0b111 << 7); // Clear INT_TYPE bits
	*GPIO_PIN1_REG |= (0b11 << 7);	 // Set INT_TYPE to any edge
	*GPIO_PIN1_REG |= (1 << 13);	 // Enable interrupt (CPU interrupt enabled)
}

void sendTrigger()
{
	*GPIO_OUT_REG |= (1 << TRIG);
	for (int i = 0; i < 1000; i++)
	{
	}
	*GPIO_OUT_REG &= ~(1 << TRIG);
}