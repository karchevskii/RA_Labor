#pragma once

#include <stdint.h>

#define I2C_SAR_ADC 0X69
#define I2C_SAR_ADC_HOSTID 0
#define ADC_SAR1_INITIAL_CODE_HIGH_ADDR 0x1
#define ADC_SAR1_INITIAL_CODE_HIGH_ADDR_MSB 0x3
#define ADC_SAR1_INITIAL_CODE_HIGH_ADDR_LSB 0x0

#define ADC_SAR1_INITIAL_CODE_LOW_ADDR 0x0
#define ADC_SAR1_INITIAL_CODE_LOW_ADDR_MSB 0x7
#define ADC_SAR1_INITIAL_CODE_LOW_ADDR_LSB 0x0

#define ADC_SAR2_INITIAL_CODE_HIGH_ADDR 0x4
#define ADC_SAR2_INITIAL_CODE_HIGH_ADDR_MSB 0x3
#define ADC_SAR2_INITIAL_CODE_HIGH_ADDR_LSB 0x0

#define ADC_SAR2_INITIAL_CODE_LOW_ADDR 0x3
#define ADC_SAR2_INITIAL_CODE_LOW_ADDR_MSB 0x7
#define ADC_SAR2_INITIAL_CODE_LOW_ADDR_LSB 0x0

/* Access internal registers, don't use in application */
void regi2c_ctrl_write_reg_mask(uint8_t block, uint8_t host_id, uint8_t reg_add, uint8_t msb, uint8_t lsb, uint8_t data);

/* Convenience macros for the above functions, these use register definitions
 * from regi2c_xxx.h header files.
 */
#define REGI2C_WRITE_MASK(block, reg_add, indata) \
    regi2c_ctrl_write_reg_mask(block, block##_HOSTID, reg_add, reg_add##_MSB, reg_add##_LSB, indata)

/**
 * @brief ADC unit
 * added "a" suffix to resolve conflicting types error
 */
typedef enum
{
    ADC_UNIT_1a, ///< SAR ADC 1
    ADC_UNIT_2a, ///< SAR ADC 2
} adc_unita_t;

/**
 * Set the calibration result to ADC.
 *
 * @note  Different ADC units and different attenuation options use different calibration data (initial data).
 *
 * @param adc_n ADC index number.
 */
__attribute__((always_inline)) static inline void adc_ll_set_calibration_param_mod(adc_unita_t adc_n, uint32_t param)
{
    uint8_t msb = param >> 8;
    uint8_t lsb = param & 0xFF;
    if (adc_n == ADC_UNIT_1a)
    {
        REGI2C_WRITE_MASK(I2C_SAR_ADC, ADC_SAR1_INITIAL_CODE_HIGH_ADDR, msb);
        REGI2C_WRITE_MASK(I2C_SAR_ADC, ADC_SAR1_INITIAL_CODE_LOW_ADDR, lsb);
    }
    else
    {
        REGI2C_WRITE_MASK(I2C_SAR_ADC, ADC_SAR2_INITIAL_CODE_HIGH_ADDR, msb);
        REGI2C_WRITE_MASK(I2C_SAR_ADC, ADC_SAR2_INITIAL_CODE_LOW_ADDR, lsb);
    }
}

/**
 * Start the calibration of the ADC unit, unit No = 1 (SAR ADC 1) or 2 (SAR ADC 2)
 */
static inline void adc_calibrate(uint32_t no)
{
    adc_unita_t adc_n = no - 1;
    adc_ll_set_calibration_param_mod(adc_n, 1682);
}
/* Temp code end. */