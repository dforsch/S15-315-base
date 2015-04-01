#include "drv8833.h"

#define PWM_LOAD_VAL    10000
#define PWM_CHANNEL_PWM   (PWM_GEN_ACTCMPAD_LOW | PWM_GEN_ACTLOAD_HIGH | PWM_GEN_ACTZERO_NOTHING)
#define PWM_CHANNEL_LOW   (PWM_GEN_ACTCMPBD_LOW | PWM_GEN_ACTLOAD_LOW | PWM_GEN_ACTZERO_NOTHING)
#define PWM_CHANNEL_HIGH  (PWM_GEN_ACTCMPBD_HIGH | PWM_GEN_ACTLOAD_HIGH | PWM_GEN_ACTZERO_NOTHING)

#define MTR_AIN1 PB4
#define MTR_AIN2 PB5
#define MTR_BIN1 PE4
#define MTR_BIN2 PE5
#define MTR_N_FAULT PF2
#define MTR_N_SLEEP PF3

//*****************************************************************************
// Initializes the 6 pins needed to control the DRV8833
//*****************************************************************************
void  drv8833_gpioInit(void)
{
  gpio_enable_port(GPIOB_BASE);
	gpio_enable_port(GPIOE_BASE);
	gpio_enable_port(GPIOF_BASE);
	
	gpio_config_digital_enable(GPIOB_BASE, PB4|PB5);
	gpio_config_digital_enable(GPIOE_BASE, PE4|PE5);
	
	gpio_config_alternate_function(GPIOB_BASE, PB4|PB5);
	gpio_config_alternate_function(GPIOE_BASE, PE4|PE5);
	
	gpio_config_port_control(GPIOE_BASE, GPIO_PCTL_PE4_M1PWM2 | GPIO_PCTL_PE5_M1PWM3);
	gpio_config_port_control(GPIOB_BASE, GPIO_PCTL_PB4_M0PWM2 | GPIO_PCTL_PB5_M0PWM3);

	gpio_config_digital_enable(GPIOF_BASE, MTR_N_SLEEP | MTR_N_FAULT);
	gpio_config_enable_output(GPIOF_BASE, MTR_N_SLEEP);
	gpio_config_enable_input(GPIOF_BASE, MTR_N_FAULT);
	
	GPIOF->DATA = MTR_N_SLEEP;

}

//*****************************************************************************
//*****************************************************************************
void  drv8833_leftForward(uint8_t dutyCycle)
{
	pwmConfig(PWM0_BASE,1,10000,7000,7000,PWM_CHANNEL_LOW,PWM_GEN_ACTCMPBD_HIGH|PWM_GEN_ACTLOAD_HIGH|PWM_GEN_ACTZERO_HIGH);
}

//*****************************************************************************
//*****************************************************************************
void  drv8833_leftReverse(uint8_t dutyCycle)
{
	
}


//*****************************************************************************
//*****************************************************************************
void  drv8833_rightForward(uint8_t dutyCycle)
{

}

//*****************************************************************************
//*****************************************************************************
void  drv8833_rightReverse(uint8_t dutyCycle)
{

}

//*****************************************************************************
//*****************************************************************************
void  drv8833_turnLeft(uint8_t dutyCycle)
{
	drv8833_rightForward(0);
	drv8833_leftReverse(0);
}

//*****************************************************************************
//*****************************************************************************
void  drv8833_turnRight(uint8_t dutyCycle)
{
	drv8833_leftForward(0);
	drv8833_rightReverse(0);
}
