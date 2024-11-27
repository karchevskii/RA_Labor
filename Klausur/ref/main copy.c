#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp32c3_reg.h"
#include "defines.h"
#include "timer.c"
#include "ultrasonic.c"
#include "spi.c"
#include "helper.c"
#include "led.c"
#include <string.h>

#define MOSI 2	// FSPIQ data out
#define MISO -1 // Not used
#define SCLK 6	// FSPICLK Clock
#define CS -1	// Not used

#define TRIG 3 // Trigger is on the GPIO3
#define ECHO 1 // Echo is on the GPIO1


spi_device_handle_t spi_handle;

// Constants for SK6812
#define LED_COUNT 16			   // Number of LEDs in the strip
#define SPI_CLOCK_SPEED_HZ 3200000 // SK6812 data rate requires precise timing
#define BITS_PER_PIXEL 24		   // SK6812 uses 24 bits per LED (8 bits each for G, R, B)
#define RESET_PERIOD 50			   // Reset period in microseconds (at least 50µs)

#define T1H 0xF0 // Logical 1: high for ~800ns (8/10 of a byte)
#define T0H 0x80 // Logical 0: high for ~400ns

// Initialize SPI
void spi_init()
{
	spi_bus_config_t spi_config = {
		.mosi_io_num = MOSI,
		.miso_io_num = MISO,
		.sclk_io_num = SCLK,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1};

	spi_device_interface_config_t spi_device_config = {
		.clock_speed_hz = SPI_CLOCK_SPEED_HZ, // Set the SPI clock speed
		.mode = 0,							  // Mode 0: CPOL=0, CPHA=0
		.spics_io_num = CS,
		.queue_size = 1 // Transaction queue size
	};

	spi_bus_initialize(SPI2_HOST, &spi_config, SPI_DMA_CH_AUTO);
	spi_bus_add_device(SPI2_HOST, &spi_device_config, &spi_handle);
}

// Encode LED colors into SPI buffer
void encode_led_data(uint8_t *led_colors, int num_leds, uint8_t *spi_buffer)
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

// Send LED data via SPI
void spi_send_led_data(uint8_t *spi_buffer, int buffer_size)
{
	spi_transaction_t t;
	memset(&t, 0, sizeof(t));
	t.length = buffer_size * 8; // Length in bits
	t.tx_buffer = spi_buffer;
	spi_device_transmit(spi_handle, &t);
}

uint8_t led_colors[LED_COUNT * 3] = {0};


void display_leds(uint16_t led_count, uint8_t color[3])
{
	// Check the buffer size
	if (led_count > LED_COUNT)
	{
		led_count = LED_COUNT;
	}

	// Loop through each LED and set the color
	for (uint16_t i = 0; i < led_count; i++)
	{
		led_colors[i * 3] = color[0];	  // Red
		led_colors[i * 3 + 1] = color[1]; // Green
		led_colors[i * 3 + 2] = color[2]; // Blue
	}

	int spi_buffer_size = LED_COUNT * BITS_PER_PIXEL + RESET_PERIOD * (SPI_CLOCK_SPEED_HZ / 1000000);
	uint8_t *spi_buffer = malloc(spi_buffer_size);
	memset(spi_buffer, 0, spi_buffer_size);

	encode_led_data(led_colors, LED_COUNT, spi_buffer);
	spi_send_led_data(spi_buffer, spi_buffer_size);
	free(spi_buffer);
}

void setupGPIOsUltraSound()
{
	*GPIO_ENABLE_REG |= (1 << TRIG);
	*IO_MUX_GPIO1_REG |= (1 << 9);
}

int readSensor()
{
	// send 1ms impult to the TRIG
	*GPIO_OUT_REG |= (1 << TRIG);
	vTaskDelay(pdMS_TO_TICKS(1));
	*GPIO_OUT_REG &= (0 << TRIG);

	// measure time on ECHO
	while (((*GPIO_IN_REG & 0b10) >> ECHO) == 0)
	{
	}
	start_timer();
	while (((*GPIO_IN_REG & 0b10) >> ECHO) == 1)
	{
	}

	// calc distance
	int timerval = (int)read_timer();
	int distance = timerval / 58;


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





void app_main()
{
	spi_init();
	setup_timer();

	setupGPIOsUltraSound();


	while (1)
	{

		int distance = readSensor();
		int leds;
		uint8_t color[3] = {0,0,0};
		uint8_t black[3] = {0,0,0};

		switch (distance)
		{
		case -2:
			leds = LED_COUNT;
			color[0] = 30;
			break;
		case -1:
			leds = LED_COUNT;
			color[1] = 30;
			break;
		default:
			leds = LED_COUNT - (1 + ((distance - 5)*14/44));
			color[0] = 30;
			break;
		}

		printf("%d\n", leds);
		display_leds(led_colors, LED_COUNT, black);
		display_leds(led_colors, leds, color);

		vTaskDelay(pdMS_TO_TICKS(100));
	
	}

}