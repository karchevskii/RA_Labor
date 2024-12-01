#pragma once
#include "esp32c3_reg.h"

void setupTimer()
{
	*TIMG_T0CONFIG_REG &= ~(0b1 << 9); // use APB_CLK
	*TIMG_T0CONFIG_REG |= (0b1 << 30); // increase
	*TIMG_T0LOADHI_REG = 0U;		   // zero reg
	*TIMG_T0LOADLO_REG = 0U;

	*TIMG_T0CONFIG_REG &= ~(0xFFFF << 13); // divider set to 65536
	*TIMG_T0CONFIG_REG |= (0x50 << 13);	   // set divider to 80 -> CLK is 1MHz
}

void startTimer()
{
	*TIMG_T0LOAD_REG = 1U;			   // Reload start value
	*TIMG_T0CONFIG_REG |= (0b1 << 31); // Enable Timer
}

int readTimer()
{
	*TIMG_T0UPDATE_REG |= (0b1 << 31);
	while (*TIMG_T0UPDATE_REG >> 31 != 0)
	{
	}
	int result = *TIMG_T0LO_REG;
	return result;
}