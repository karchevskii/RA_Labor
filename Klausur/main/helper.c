#include "defines.h"
#include <stdint.h>

// Encode LED SK6812 colors into SPI buffer
void encodeLedSKData(uint8_t *led_colors, int num_leds, uint8_t *spi_buffer)
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

// Encode LED WS2812 colors into SPI buffer
void encodeLedWSData(uint8_t *led_colors, int num_leds, uint8_t *spi_buffer)
{
	int buffer_index = 0;
	for (int i = 0; i < num_leds; i++)
	{
		uint8_t green = led_colors[i * 3 + 0];
		uint8_t red = led_colors[i * 3 + 1];
		uint8_t blue = led_colors[i * 3 + 2];

		// WS2812 expects data in GRB order
		uint8_t grb[3] = {green, red, blue};

		for (int j = 0; j < 3; j++)
		{
			for (int bit = 7; bit >= 0; bit--)
			{
				if (grb[j] & (1 << bit))
				{
					spi_buffer[buffer_index++] = T1H_WS2812;
				}
				else
				{
					spi_buffer[buffer_index++] = T0H_WS2812;
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

int calculateBrightness(int adcVal)
{
	// minvalue is 1692, maxvalue is 4095
	int brightness = ((adcVal - ADC_MIN) * 255 / (ADC_MAX - ADC_MIN));
	return brightness;
}

int calculateDistance(int timerval)
{
	int distance = timerval / 58; // See datasheet for calculation
	if (distance < MIN_DISTANCE || distance > MAX_DISTANCE)
	{
		return -1; // Invalid distance
	}
	return distance;
}

int calculateStableDistance(int timerval)
{
	static int buffer[BUFFER_SIZE]; // Circular buffer to store recent distances
	static int bufferIndex = 0;		// Current index in the buffer
	static int bufferCount = 0;

	int newDistance = calculateDistance(timerval);

	buffer[bufferIndex] = newDistance;
	bufferIndex = (bufferIndex + 1) % BUFFER_SIZE;

	if (bufferCount < BUFFER_SIZE)
	{
		bufferCount++;
	}

	int sum = 0;

	for (int i = 0; i < bufferCount; i++)
	{
		sum += buffer[i];
	}

	int stableDistance = sum / bufferCount;
	if (stableDistance < 0)
	{
		stableDistance = -1;
	}
	

	return stableDistance;
}