#pragma once
#define MOSI 2	// FSPIQ data out
#define MISO -1 // Not used
#define SCLK 6	// FSPICLK Clock
#define CS -1	// Not used

#define MOSI_ONBOARD_LED 8

#define TRIG 3 // Trigger is on the GPIO3
#define ECHO 1 // Echo is on the GPIO1


// Constants for SK6812
#define LED_COUNT 16			   // Number of LEDs in the strip
#define SPI_CLOCK_SPEED_HZ 3200000 // SK6812 data rate requires precise timing
#define BITS_PER_PIXEL 24		   // SK6812 uses 24 bits per LED (8 bits each for G, R, B)
#define RESET_PERIOD 50			   // Reset period in microseconds (at least 50µs)

// SK6812 timing constants
#define T1H 0xF0 // Logical 1
#define T0H 0x80 // Logical 0

// WS2812 timing constants
#define T1H_WS2812 0xE0 // Logical 1
#define T0H_WS2812 0x80 // Logical 0

#define GPIO_INTERRUPT_SRC 16

#define MIN_DISTANCE 2     // Minimum valid distance in cm
#define MAX_DISTANCE 400   // Maximum valid distance in cm
#define SMOOTHING_FACTOR 95 // Weight for new data in exponential smoothing