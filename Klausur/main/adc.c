#include <stdint.h>
#include <stdio.h>
#include "esp32c3_reg.h"
#include "calibrate_adc.h"

#include <inttypes.h>

void initADC()
{
    // *GPIO_ENABLE_REG &= 0b0;       // disable output for pin 0
    *IO_MUX_GPIO0_REG &= ~(0b1 << 9); // Set FUN_IE to HIGH
    // *IO_MUX_GPIO0_REG |= 0b1 << 4;                // input enable in sleep mode
    *IO_MUX_GPIO0_REG &= ~(0b1 << 7); // WPD disable
    *IO_MUX_GPIO0_REG &= ~(0b1 << 8); // WPU disable

    *APB_SARADC_CTRL_REG |= 0b1 << 0; // ADC durch Software
    *APB_SARADC_CTRL_REG |= 0b1 << 1; // start ADC

    // // enable clk
    // *SYSTEM_PERIP_CLK_EN0_REG |= 1 << 28;
    // *SYSTEM_PERIP_RST_EN0_REG |= 1 << 28;
    // *SYSTEM_PERIP_RST_EN0_REG &= ~(1 << 28);

    *APB_SARADC_ONETIME_SAMPLE_REG &= ~(0b1111 << 25); // channel 0
    *APB_SARADC_ONETIME_SAMPLE_REG |= 0b1 << 31; // enable one time sample adc1
    *APB_SARADC_ONETIME_SAMPLE_REG |= 0b11 << 23; // set 3 for 12dB attenuation
    *APB_SARADC_INT_ENA_REG |= 0b1 << 31;        // DONE interrupt

    *APB_SARADC_INT_CLR_REG |= (0b1 << 31); // reset interrupt
}

uint32_t readADC()
{
    *APB_SARADC_ONETIME_SAMPLE_REG |= 0b1 << 29; // start one time sample
    for (int i = 0; i < 10000; i++)
    {
    }
    *APB_SARADC_ONETIME_SAMPLE_REG &= ~(0b1 << 29); // stop one time sample
    while ((*APB_SARADC_INT_RAW_REG & (0b1 << 31)) == 0U)
    {
    }
    uint32_t data = *APB_SARADC_1_DATA_STATUS_REG & 0xFFFU;
    *APB_SARADC_INT_CLR_REG |= (0b1 << 31); // reset interrupt
    return data;
}