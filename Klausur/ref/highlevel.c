#include "driver/spi_master.h"
#include "esp32c3_reg.h"
#include "defines.h"
#include <string.h>
#include <stdio.h>

#include <esp_intr_alloc.h>
#include "driver/gpio.h"

volatile int cause = 0;

void setupGPIO() {
  gpio_config_t echo_config = {
    .pin_bit_mask = (1ULL << ECHO),
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_ANYEDGE
  };
  gpio_config(&echo_config);
}


static void IRAM_ATTR gpio_isr_handler(void* arg) {
  cause = (uint32_t)arg;
  uint32_t gpio_num = (uint32_t)arg;
}

void app_main() {
  // Initialize GPIO
  setupGPIO();

  // Attach interrupt handler for ECHO pin
  gpio_install_isr_service(0);
  // Attach interrupt handler for ECHO pin
  gpio_isr_handler_add(ECHO, gpio_isr_handler, (void*)ECHO);

  gpio_dump_io_configuration(stdout, BIT(ECHO));

  uint32_t procpu_int = *GPIO_PCPU_INT_REG;  // Read the value of GPIO_PCPU_INT_REG
  int gpio2_status = (procpu_int >> ECHO) & 0x1;  // Extract bit for GPIO2

  printf("GPIO2 Interrupt Status: %d\n", gpio2_status);

  // Main loop
  while (1) {
    printf("cause %d\n", cause);
    int stat = (*GPIO_STATUS_REG & 0b100) >> 2;  // Read the value of 

    printf("GPIO2 Status: %d\n", stat);

    for (int i = 0; i < 4000000; i++)
    {
      /* code */
    }
  }
}