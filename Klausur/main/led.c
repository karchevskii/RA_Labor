#pragma once

#include <string.h>
#include <stdint.h>
#include "defines.h"

void displayLeds(uint8_t *led_colors, uint16_t led_count, uint8_t color[3])
{

    // Check the buffer size
    if (led_count > LED_COUNT)
    {
        led_count = LED_COUNT;
    }

    // Loop through each LED and set the color, this function is implemented in asm below
    // for (uint16_t i = 0; i < led_count; i++)
    // {
    //     led_colors[i * 3] = color[0];     // Red
    //     led_colors[i * 3 + 1] = color[1]; // Green
    //     led_colors[i * 3 + 2] = color[2]; // Blue
    // }

    // in asm
    asm volatile(
        // Zero the index (t0) and load led_count into t1
        "li t0, 0 \n\t"             // t0 = index i = 0
        "mv t1, %[led_count] \n\t"  // t1 = led_count
        "mv t2, %[color] \n\t"      // t2 = color base address
        "mv t3, %[led_colors] \n\t" // t3 = led_colors base address

        "loop: \n\t"            // Loop label
        "bge t0, t1, exit \n\t" // Exit loop if i >= led_count

        // Calculate address for led_colors[i * 3]
        "slli t4, t0, 1 \n\t" // t4 = i * 2
        "add t4, t4, t0 \n\t" // t4 = i * 3
        "add t4, t4, t3 \n\t" // t4 = &led_colors[i * 3]

        // Set LED colors
        "lb t5, 0(t2) \n\t" // Load red color (color[0])
        "sb t5, 0(t4) \n\t" // Store red into led_colors[i * 3]
        "lb t5, 1(t2) \n\t" // Load green color (color[1])
        "sb t5, 1(t4) \n\t" // Store green into led_colors[i * 3 + 1]
        "lb t5, 2(t2) \n\t" // Load blue color (color[2])
        "sb t5, 2(t4) \n\t" // Store blue into led_colors[i * 3 + 2]

        // Increment index
        "addi t0, t0, 1 \n\t" // i++

        // Repeat the loop
        "j loop \n\t" // Jump to loop label

        "exit: \n\t" // Exit label

        :
        : [led_colors] "r"(led_colors), [led_count] "r"(led_count), [color] "r"(color)
        : "t0", "t1", "t2", "t3", "t4", "t5");

    // set other LEDs black
    for (uint16_t i = led_count; i <= LED_COUNT; i++)
    {
        led_colors[i * 3] = 0;     // Red
        led_colors[i * 3 + 1] = 0; // Green
        led_colors[i * 3 + 2] = 0; // Blue
    }

    int spi_buffer_size = LED_COUNT * BITS_PER_PIXEL + RESET_PERIOD * (SPI_CLOCK_SPEED_HZ / 1000000);
    uint8_t *spi_buffer = malloc(spi_buffer_size);
    memset(spi_buffer, 0, spi_buffer_size);

    encodeLedSKData(led_colors, LED_COUNT, spi_buffer);
    spiSendToDevice(spi_buffer, spi_buffer_size);
    free(spi_buffer);
}

void displayOnboardLed(uint8_t *led_colors, uint16_t led_count, uint8_t color[3])
{

    // Check the buffer size
    if (led_count > LED_COUNT)
    {
        led_count = LED_COUNT;
    }

    // Loop through each LED and set the color, this function is implemented in asm below
    for (uint16_t i = 0; i < led_count; i++)
    {
        led_colors[i * 3] = color[0];     // Red
        led_colors[i * 3 + 1] = color[1]; // Green
        led_colors[i * 3 + 2] = color[2]; // Blue
    }

    // set other LEDs black
    for (uint16_t i = led_count; i <= LED_COUNT; i++)
    {
        led_colors[i * 3] = 0;     // Red
        led_colors[i * 3 + 1] = 0; // Green
        led_colors[i * 3 + 2] = 0; // Blue
    }

    int spi_buffer_size = LED_COUNT * BITS_PER_PIXEL + RESET_PERIOD * (SPI_CLOCK_SPEED_HZ / 1000000);
    uint8_t *spi_buffer = malloc(spi_buffer_size);
    memset(spi_buffer, 0, spi_buffer_size);

    encodeLedWSData(led_colors, LED_COUNT, spi_buffer);
    spiSendToDevice(spi_buffer, spi_buffer_size);
    free(spi_buffer);
}