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


uint8_t led_colors[LED_COUNT * 3] = {0};

void app_main()
{
	initADC();
	spi_init();
	setup_timer();

	setupGPIOsUltraSound();

	inbuiltLED();

	while (1)
	{

		int distance = readSensor();
		int leds;
		uint8_t color[3] = {0,0,0};

		int brightness = calculateBrightness((int)adc_read());

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
			leds = LED_COUNT - (1 + ((distance - 5)*14/44));
			color[0] = brightness;
			break;
		}

		display_leds(led_colors, leds, color);

		for (int i = 0; i < 4000000; i++)
		{
			/* code */
		}
		
	}

}