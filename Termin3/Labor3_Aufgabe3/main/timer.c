#include "esp32c3_reg.h"

void setup_timer()
{
	*TIMG_T0CONFIG_REG &= ~(0b1 << 9); // use APB_CLK
	*TIMG_T0CONFIG_REG |= (0b1 << 30); // increase
	//*TIMG_T0CONFIG_REG |= (0b1 << 29); // autoreload
	*TIMG_T0LOADLO_REG = 0U;

	*TIMG_T0CONFIG_REG &= ~(0xFFFF << 13); // divider set to 65536
	*TIMG_T0CONFIG_REG |= (1 << 13);	   // set divider to 2 (default val)

	*TIMG_T0LOAD_REG = 1U; // Reload start value

	*TIMG_T0ALARMLO_REG = 400000;	   // Set Alarm Counter to 400000 -> 200Hz: 80MHz/200Hz=400k
	*TIMG_T0LOADHI_REG &= ~0x3FFFF;	   // zero reg
	*TIMG_T0CONFIG_REG |= (0b1 << 31); // Enable Timer

	*TIMG_INT_ENA_TIMERS_REG |= 0b1; // interrupt enable
	*TIMG_T0CONFIG_REG |= (0b1 << 10); // set alarm
}



int calcCounter(int adcVal) {

	// smallest adcVal is 30, biggest is 4095
	int frequenz = 200 + ((adcVal - 30)*800/4065);
    int counter = 800000/frequenz; 
	return counter;
}

void set_timer(int adcVal){
    //calc freq
    *TIMG_T0ALARMLO_REG = calcCounter(adcVal);	   // Set Alarm Counter 
    *TIMG_T0LOADHI_REG &= ~0x3FFFF;	   // zero reg
}
