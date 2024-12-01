#include "defines.h"
#include <stdint.h>

// Encode LED colors into SPI buffer
void encodeLedData(uint8_t *led_colors, int num_leds, uint8_t *spi_buffer)
{
	int buffer_index = 0;
	for (int i = 0; i < num_leds; i++)
	{
		uint8_t green = led_colors[i * 3 + 0];
		uint8_t red = led_colors[i * 3 + 1];
		uint8_t blue = led_colors[i * 3 + 2];

		// SK6812 expects data in GRB order
		uint8_t grb[3] = {green, red, blue};

		for (int j = 0; j < 3; j++)
		{
			for (int bit = 7; bit >= 0; bit--)
			{
				if (grb[j] & (1 << bit))
				{
					spi_buffer[buffer_index++] = T1H;
				}
				else
				{
					spi_buffer[buffer_index++] = T0H;
				}
			}
		}
	}

	// Add reset period (50µs)
	for (int i = 0; i < RESET_PERIOD * (SPI_CLOCK_SPEED_HZ / 1000000); i++)
	{
		spi_buffer[buffer_index++] = 0;
	}
}


int calculateBrightness(int adcVal){
	//minvalue is 1692, maxvalue is 4095
	int brightness = ((adcVal - 1692)*255/2403);
	return brightness;
}

int calculateDistance(int timerval){
	int distance = timerval / 58; //see datasheet
	if (distance >= 50)
	{
		return -1;
	}
	if (distance < 5)
	{
		return -2;
	}
	return distance;
}