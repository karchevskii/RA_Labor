#include "esp32c3_reg.h"
#include "defines.h"


void setupGPIOsUltraSound()
{
	*GPIO_ENABLE_REG |= (1 << TRIG);
	*IO_MUX_GPIO1_REG |= (1 << 9);
}

int readSensor()
{
	// send 1ms impult to the TRIG
	*GPIO_OUT_REG |= (1 << TRIG);
	
    for (int i = 0; i < 1000; i++)
    {
        
    }
    

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