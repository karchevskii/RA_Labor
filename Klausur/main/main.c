#include "driver/spi_master.h"
#include "esp32c3_reg.h"
#include "defines.h"
#include "timer.c"
#include "ultrasonic.c"
#include "spi.c"
#include "helper.c"
#include "led.c"
#include "adc.c"
#include <string.h>
#include <esp_intr_alloc.h>

uint8_t led_colors[LED_COUNT * 3] = {0};

int interrupt_triggered = 0;
int distance = 0;

static void IRAM_ATTR gpioIsrHandler()
{
	int interrupt_echo = (*GPIO_STATUS_REG & 0b10) >> ECHO; // check whether the ECHO was the cause of the interrupt

	if (!interrupt_echo) // something went wrong -> we expect interrupts only from ECHO
	{
		*GPIO_STATUS_W1TC_REG |= 0xFFFF; // clear all interrupts
		return;							 // end the ISR
	}

	if (!interrupt_triggered) // rising edge -> start timer
	{
		startTimer();
		interrupt_triggered = 1; // set flag to 1 (toggle) to detect the next edge is falling
	}
	else // falling edge -> end timer and calculate distance
	{
		int timerval = (int)readTimer(); // read timer
		distance = calculateDistance(timerval);
		interrupt_triggered = 0; // toggle flag to 0 -> next edge is rising
	}
	*GPIO_STATUS_W1TC_REG |= 0b10; // clear the interrupt of ECHO
}

void app_main()
{
	initADC();
	setupTimer();
	initSPI(MOSI);
	setupGPIOsUltrasonic();

	esp_intr_alloc(GPIO_INTERRUPT_SRC, 0, gpioIsrHandler, NULL, NULL);

	// inbuiltLED();

	while (1)
	{
		sendTrigger();
		int leds;
		uint8_t color[3] = {0, 0, 0};

		

		int brightness = calculateBrightness((int)readADC());

		switch (distance)
		{
		case -2:
			leds = LED_COUNT;
			color[0] = brightness;
			break;
		case -1:
			leds = LED_COUNT;
			color[1] = brightness;
			break;
		default:
			leds = LED_COUNT - (1 + ((distance - 5) * 14 / 44));
			color[0] = brightness;
			break;
		}

		displayOnboardLed(led_colors, leds, color);
		changeMOSI(MOSI_ONBOARD_LED);
		displayOnboardLed(led_colors, 1, color);
		changeMOSI(MOSI);

		for (int i = 0; i < 4000000; i++)
		{
			/* code */
		}	
	}
}