#include "driver/spi_master.h"
#include <string.h>
#include "defines.h"

spi_device_handle_t spi_handle;

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


// Send LED data via SPI
void spi_send_led_data(uint8_t *spi_buffer, int buffer_size)
{
	spi_transaction_t t;
	memset(&t, 0, sizeof(t));
	t.length = buffer_size * 8; // Length in bits
	t.tx_buffer = spi_buffer;
	spi_device_transmit(spi_handle, &t);
}