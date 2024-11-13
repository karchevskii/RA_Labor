/*
	Control a 7-segment display 
	at GPIO Pins 1 to 7
*/

#include <stdint.h>
#include "esp32c3_regs.h"

// Define the LED digit patters, from 0 - 9
// 1 = LED on, 0 = LED off, in this order:
// Segment A, B, C, D, E, F, G
uint8_t seven_seg_digits[10][7] = { 
   { 1,1,1,1,1,1,0 },  // = 0
   { 0,1,1,0,0,0,0 },  // = 1
   { 1,1,0,1,1,0,1 },  // = 2
   { 1,1,1,1,0,0,1 },  // = 3
   { 0,1,1,0,0,1,1 },  // = 4
   { 1,0,1,1,0,1,1 },  // = 5
   { 1,0,1,1,1,1,1 },  // = 6
   { 1,1,1,0,0,0,0 },  // = 7
   { 1,1,1,1,1,1,1 },  // = 8
   { 1,1,1,0,0,1,1 }   // = 9
};

void initDisplay() {               
   *GPIO_ENABLE_REG |= 0b11111110;  	// enable output of GPIO pins 1..7 for 7 segments
   *IO_MUX_GPIO4_REG |= (1 << 12); 		// set IO Mux function 1 (see Table 5.3 of Techn. Ref. Manual) 
   *IO_MUX_GPIO5_REG |= (1 << 12); 		// set IO Mux function 1 (see Table 5.3 of Techn. Ref. Manual) 
   *IO_MUX_GPIO6_REG |= (1 << 12); 		// set IO Mux function 1 (see Table 5.3 of Techn. Ref. Manual) 
   *IO_MUX_GPIO7_REG |= (1 << 12); 		// set IO Mux function 1 (see Table 5.3 of Techn. Ref. Manual) 
}

void writeDisplay(uint32_t segment, uint8_t bit) {
  if (bit == 1)
    *GPIO_OUT_W1TS_REG = 0b10 << segment;
  else
    *GPIO_OUT_W1TC_REG = 0b10 << segment;
}
   
void sevenSegWrite(uint32_t digit) {
   int segment;
   for (segment = 0; segment < 7; segment++) {
     writeDisplay(segment, seven_seg_digits[digit][segment]);
   }
}
