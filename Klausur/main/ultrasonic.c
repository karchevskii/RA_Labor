#include "esp32c3_reg.h"
#include "defines.h"
#include <stdio.h>

void setupGPIOsUltrasonic()
{
	volatile uint32_t *ECHO_PIN_REG = (volatile uint32_t *)(GPIO_PIN0_REG + ECHO);
	volatile uint32_t *ECHO_IO_MUX_REG = (volatile uint32_t *)(IO_MUX_GPIO0_REG + ECHO);

	// Set TRIG as output
	*GPIO_ENABLE_REG |= (1 << TRIG);

	// Set ECHO as input and configure for interrupt
	*ECHO_IO_MUX_REG |= (1 << 9);	// Set FUN_IE to HIGH
	*ECHO_IO_MUX_REG |= (1 << 12);	// Set IO_MUX Func1
	*ECHO_PIN_REG &= ~(0b111 << 7); // Clear INT_TYPE bits
	*ECHO_PIN_REG |= (0b11 << 7);	// Set INT_TYPE to any edge
	*ECHO_PIN_REG |= (1 << 13);		// Enable interrupt (CPU interrupt enabled)
}

void sendTrigger()
{
	*GPIO_OUT_REG |= (1 << TRIG);
	for (int i = 0; i < 1000; i++)
	{
	}
	*GPIO_OUT_REG &= ~(1 << TRIG);
}
