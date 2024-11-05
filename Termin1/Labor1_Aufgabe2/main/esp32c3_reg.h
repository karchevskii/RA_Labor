# include <stdint.h>

// GPIO Configuration Registers
volatile uint32_t* GPIO_BASE            =   (volatile uint32_t*) 0x60004000;            // GPIO Base Register
volatile uint32_t* GPIO_OUT_W1TS_REG    =   (volatile uint32_t*) 0x60004008;            // GPIO Output Set Register
volatile uint32_t* GPIO_OUT_W1TC_REG    =   (volatile uint32_t*) 0x6000400C;            // GPIO Output Clear Register
volatile uint32_t* GPIO_ENABLE_REG      =   (volatile uint32_t*) 0x60004020;            // GPIO Enable Register
volatile uint32_t* GPIO_ENABLE_W1TC_REG =   (volatile uint32_t*) 0x60009028;

//GPIO Input Function Configuration Registers
volatile uint32_t* GPIO_FUNC0_OUT_SEL_CFG_REG =   (volatile uint32_t*) 0x60004554;
volatile uint32_t* GPIO_FUNC1_OUT_SEL_CFG_REG =   (volatile uint32_t*) 0x60004558;
volatile uint32_t* GPIO_FUNC2_OUT_SEL_CFG_REG =   (volatile uint32_t*) 0x6000455C;
volatile uint32_t* GPIO_FUNC3_OUT_SEL_CFG_REG =   (volatile uint32_t*) 0x60004560;
volatile uint32_t* GPIO_FUNC4_OUT_SEL_CFG_REG =   (volatile uint32_t*) 0x60004564;
volatile uint32_t* GPIO_FUNC5_OUT_SEL_CFG_REG =   (volatile uint32_t*) 0x60004568;
volatile uint32_t* GPIO_FUNC6_OUT_SEL_CFG_REG =   (volatile uint32_t*) 0x6000456C;
volatile uint32_t* GPIO_FUNC7_OUT_SEL_CFG_REG =   (volatile uint32_t*) 0x60004570;
volatile uint32_t* GPIO_FUNC8_OUT_SEL_CFG_REG =   (volatile uint32_t*) 0x60004574;
volatile uint32_t* GPIO_FUNC9_OUT_SEL_CFG_REG =   (volatile uint32_t*) 0x60004578;
volatile uint32_t* GPIO_FUNC10_OUT_SEL_CFG_REG =  (volatile uint32_t*) 0x6000457C;

// IO MUX Configuration Registers
volatile uint32_t* IO_MUX_BASE      =   (volatile uint32_t*) 0x60009000;
volatile uint32_t* IO_MUX_GPIO0_REG =   (volatile uint32_t*) 0x60009004;
volatile uint32_t* IO_MUX_GPIO1_REG =   (volatile uint32_t*) 0x60009008;
volatile uint32_t* IO_MUX_GPIO2_REG =   (volatile uint32_t*) 0x6000900C;
volatile uint32_t* IO_MUX_GPIO3_REG =   (volatile uint32_t*) 0x60009010;
volatile uint32_t* IO_MUX_GPIO4_REG =   (volatile uint32_t*) 0x60009014;
volatile uint32_t* IO_MUX_GPIO5_REG =   (volatile uint32_t*) 0x60009018;
volatile uint32_t* IO_MUX_GPIO6_REG =   (volatile uint32_t*) 0x6000901C;
volatile uint32_t* IO_MUX_GPIO7_REG =   (volatile uint32_t*) 0x60009020;
volatile uint32_t* IO_MUX_GPIO8_REG =   (volatile uint32_t*) 0x60009024;
volatile uint32_t* IO_MUX_GPIO9_REG =   (volatile uint32_t*) 0x60009028;
volatile uint32_t* IO_MUX_GPIO10_REG =  (volatile uint32_t*) 0x6000902C;

#define IO_MUX_GPIOn_FUN_DRV 		10
#define IO_MUX_GPIOn_MCU_SEL 		12
#define IO_MUX_PIN_FUNC_GPIO 		1
#define IO_MUX_PIN_DRV_40mA 		3

// TIMERSETUP
volatile uint32_t* TIMG_T0CONFIG_REG        =   (volatile uint32_t*) 0x6001F000;
volatile uint32_t* TIMG_T0LO_REG            =   (volatile uint32_t*) 0x6001F004;
volatile uint32_t* TIMG_T0HI_REG            =   (volatile uint32_t*) 0x6001F008;
volatile uint32_t* TIMG_T0UPDATE_REG        =   (volatile uint32_t*) 0x6001F00C;
volatile uint32_t* TIMG_T0ALARMLO_REG       =   (volatile uint32_t*) 0x6001F010;
volatile uint32_t* TIMG_T0ALARMHI_REG       =   (volatile uint32_t*) 0x6001F014;
volatile uint32_t* TIMG_T0LOADLO_REG        =   (volatile uint32_t*) 0x6001F018;
volatile uint32_t* TIMG_T0LOADHI_REG        =   (volatile uint32_t*) 0x6001F01C;
volatile uint32_t* TIMG_T0LOAD_REG          =   (volatile uint32_t*) 0x6001F020;
volatile uint32_t* TIMG_INT_ENA_TIMERS_REG  =   (volatile uint32_t*) 0x6001F070;
volatile uint32_t* TIMG_INT_RAW_TIMERS_REG  =   (volatile uint32_t*) 0x6001F074;
volatile uint32_t* TIMG_INT_CLR_TIMERS_REG  =   (volatile uint32_t*) 0x6001F07C;

//PWM
volatile uint32_t* LED_PWM_BASE             =   (volatile uint32_t*) 0x60019000;
volatile uint32_t* LEDC_CH0_CONF0_REG       =   (volatile uint32_t*) 0x60019000;        //Configuration register 0 for channel 0
volatile uint32_t* LEDC_CH0_CONF1_REG       =   (volatile uint32_t*) 0x6001900C;        //Configuration register 1 for channel 0
volatile uint32_t* LEDC_CH0_HPOINT_REG      =   (volatile uint32_t*) 0x60019004;        //High point register for channel 0
volatile uint32_t* LEDC_CH0_DUTY_REG        =   (volatile uint32_t*) 0x60019008;        //Initial duty cycle for channel 0
volatile uint32_t* LEDC_CH0_DUTY_R_REG      =   (volatile uint32_t*) 0x60019010;        //Current duty cycle for channel 0
volatile uint32_t* LEDC_TIMER0_CONF_REG     =   (volatile uint32_t*) 0x600190A0;        //Timer 0 configuration
volatile uint32_t* LEDC_TIMER0_VALUE_REG    =   (volatile uint32_t*) 0x600190A4;        //Timer 0 current counter value
volatile uint32_t* LEDC_CONF_REG            =   (volatile uint32_t*) 0x600190D0;        //Global LEDC configuration register

//System
volatile uint32_t* SYSTEM_CPU_PERI_CLK_EN_REG =   (volatile uint32_t*) 0x600C0000;
volatile uint32_t* SYSTEM_CPU_PERI_RST_EN_REG =   (volatile uint32_t*) 0x600C0004;
volatile uint32_t* SYSTEM_PERIP_CLK_EN0_REG   =   (volatile uint32_t*) 0x600C0010;
volatile uint32_t* SYSTEM_PERIP_CLK_EN1_REG   =   (volatile uint32_t*) 0x600C0014;
volatile uint32_t* SYSTEM_PERIP_RST_EN0_REG   =   (volatile uint32_t*) 0x600C0018;
volatile uint32_t* SYSTEM_PERIP_RST_EN1_REG   =   (volatile uint32_t*) 0x600C001C;

// I2C Setup
volatile uint32_t* I2C_BASE                     =   (volatile uint32_t*) 0x60013000;
volatile uint32_t* I2C_CTR_REG                  =   (volatile uint32_t*) 0x60013004;     //page 692 techn. ref.
volatile uint32_t* I2C_SLAVE_ADDR_REG           =   (volatile uint32_t*) 0x60013010;
volatile uint32_t* I2C_COMD0_REG                =   (volatile uint32_t*) 0x60013058;
volatile uint32_t* I2C_COMD1_REG                =   (volatile uint32_t*) 0x6001305C;
volatile uint32_t* I2C_COMD2_REG                =   (volatile uint32_t*) 0x60013060;

// ADC Configuration Registers
volatile uint32_t* APB_SARADC_CTRL_REG                  =   (volatile uint32_t*) 0x60040000;     // SAR ADC control register 1
volatile uint32_t* APB_SARADC_CTRL2_REG                 =   (volatile uint32_t*) 0x60040004;    // SAR ADC control register 2
volatile uint32_t* APB_SARADC_ONETIME_SAMPLE_REG        =   (volatile uint32_t*) 0x60040020;   // Configuration register for one-time sampling
volatile uint32_t* APB_SARADC_1_DATA_STATUS_REG         =   (volatile uint32_t*) 0x6004002C;    // SAR ADC1 sampling data register
volatile uint32_t* APB_SARADC_APB_ADC_CLKM_CONF_REG     =   (volatile uint32_t*) 0x60040054;    // SAR ADC clock control register
volatile uint32_t* APB_SARADC_INT_ENA_REG               =   (volatile uint32_t*) 0x60040040;  // Enable register of SAR ADC interrupts
volatile uint32_t* APB_SARADC_INT_RAW_REG               =   (volatile uint32_t*) 0x60040044;  // Raw register of SAR ADC interrupts
volatile uint32_t* APB_SARADC_INT_CLR_REG               =   (volatile uint32_t*) 0x6004004C;  // Clear register of SAR ADC interrupts
