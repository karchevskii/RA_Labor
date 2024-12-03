#include "driver/spi_master.h"
#include "esp32c3_reg.h"
#include "defines.h"
#include "timer.c"
#include "ultrasonic.c"
#include "led.c"
#include "adc.c"
#include <string.h>
#include <esp_intr_alloc.h>

uint8_t led_colors[LED_COUNT * 3] = {0};
int leds;
// static int currentDistance = 0; // Keep track of the smoothed distance

int interrupt_triggered = 0;
int distance = 0;
int real_distance = 0;

static void IRAM_ATTR gpioIsrHandler()
{
	int interrupt_echo = (*GPIO_STATUS_REG >> ECHO) & 0b1; // check whether the ECHO was the cause of the interrupt

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
		distance = calculateStableDistance(timerval);
		real_distance = calculateDistance(timerval);
		interrupt_triggered = 0; // toggle flag to 0 -> next edge is rising
	}
	*GPIO_STATUS_W1TC_REG |= (0b1 << ECHO); // clear the interrupt of ECHO
}

void app_main()
{
	initADC();
	if (DEBUG && SETUP)
		setupADC();
	setupTimer();
	initSPI(MOSI);
	setupGPIOsUltrasonic();

	esp_intr_alloc(GPIO_INTERRUPT_SRC, 0, gpioIsrHandler, NULL, NULL);

	while (1)
	{
		sendTrigger();

		uint8_t color_led_strip[3] = {0, 0, 0};
		uint8_t color_onboard_led[3] = {0, 0, 0};

		int brightness = calculateBrightness((int)readADC());

		if (distance < 5 && distance >= 0)
		{ // distance < 5cm and positive or 0
			leds = LED_COUNT;
			color_led_strip[0] = brightness;
			if (distance < 3) // display red
			{
				color_onboard_led[1] = brightness;
			}
			else if (distance < 4) // display yellow
			{
				color_onboard_led[0] = brightness;
				color_onboard_led[1] = brightness;
			}
			else // display green
			{
				color_onboard_led[0] = brightness;
			}
		}
		else if (distance >= 50) // distance >= 50cm
		{
			leds = LED_COUNT;
			color_led_strip[1] = brightness;
		}
		else if (distance == -1) // error
		{
			leds = LED_COUNT;
			color_led_strip[2] = brightness;
			color_onboard_led[2] = brightness;
		}
		else
		{ // Rainbow gradient for 5cm <= distance < 50cm
		  // Map distance to hue range [0, 255]
			uint8_t red = 0, green = 0, blue = 0;

			int range = 50 - 5;
			int position = distance - 5;
			int hue = (position * 255) / range;

			// Convert hue to RGB (rainbow colors)
			if (hue < 85)
			{												// Red to yellow to green
				red = (255 - (hue * 3)) * brightness / 255; // Decrease red
				green = (hue * 3) * brightness / 255;		// Increase green
				blue = 0;
			}
			else if (hue < 170)
			{ // Green to cyan to blue
				hue -= 85;
				red = 0;
				green = (255 - (hue * 3)) * brightness / 255; // Decrease green
				blue = (hue * 3) * brightness / 255;		  // Increase blue
			}
			else
			{ // Blue to purple to red
				hue -= 170;
				red = (hue * 3) * brightness / 255; // Increase red
				green = 0;
				blue = (255 - (hue * 3)) * brightness / 255; // Decrease blue
			}
			color_led_strip[0] = red;
			color_led_strip[1] = green;
			color_led_strip[2] = blue;
			leds = LED_COUNT - (1 + ((distance - 5) * (LED_COUNT - 1) / range));
		}

		displayLeds(led_colors, leds, color_led_strip);
		changeMOSI(MOSI_ONBOARD_LED);
		displayOnboardLed(led_colors, 1, color_onboard_led);
		changeMOSI(MOSI);

		// for debug:
		if (DEBUG)
		{
			printf("Distance: %d cm\n", distance);
			printf("real Distance: %d cm\n", real_distance);
			printf("Brightness: %d\n", brightness);
			printf("leds: %d\n", leds);
			printf("\n");
		}

		for (int i = 0; i < 100000; i++)
		{
		}
	}
}