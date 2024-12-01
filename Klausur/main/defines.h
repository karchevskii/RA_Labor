#pragma once
#define MOSI 2	// FSPIQ data out
#define MISO -1 // Not used
#define SCLK 6	// FSPICLK Clock
#define CS -1	// Not used

#define TRIG 3 // Trigger is on the GPIO3
#define ECHO 1 // Echo is on the GPIO1
#define INBUILT_LED 8


// Constants for SK6812
#define LED_COUNT 16			   // Number of LEDs in the strip
#define SPI_CLOCK_SPEED_HZ 3200000 // SK6812 data rate requires precise timing
#define BITS_PER_PIXEL 24		   // SK6812 uses 24 bits per LED (8 bits each for G, R, B)
#define RESET_PERIOD 50			   // Reset period in microseconds (at least 50µs)

#define T1H 0xF0 // Logical 1: high for ~800ns (8/10 of a byte)
#define T0H 0x80 // Logical 0: high for ~400ns (4/10 of a byte)

#define GPIO_INTERRUPT_SRC 16
