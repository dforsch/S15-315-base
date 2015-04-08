#include "drv8833.h"
#include "../include/sysctrl.h"

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

#define LEFT_MOTOR				PWM0_BASE, 1
#define LEFT_FORWARDS			PWM_CHANNEL_PWM_A, PWM_CHANNEL_LOW
#define LEFT_BACKWARDS		PWM_CHANNEL_LOW, PWM_CHANNEL_PWM_B
#define LEFT_STOP					PWM_CHANNEL_LOW, PWM_CHANNEL_LOW
#define RIGHT_MOTOR				PWM1_BASE, 1
#define RIGHT_FORWARDS		LEFT_FORWARDS
#define RIGHT_BACKWARDS		LEFT_BACKWARDS
#define RIGHT_STOP				PWM_CHANNEL_LOW, PWM_CHANNEL_LOW

//*****************************************************************************
// Initializes the 6 pins needed to control the DRV8833
//*****************************************************************************
void  drv8833_gpioInit(void)
{
	SYSCTL->RCGCPWM |= SYSCTL_RCGCPWM_R0 | SYSCTL_RCGCPWM_R1;
	while((SYSCTL->PRPWM & SYSCTL_PRPWM_R0) == 0 || (SYSCTL->PRPWM & SYSCTL_PRPWM_R1) == 0){
	}
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
	
	GPIOF->DATA |= MTR_N_SLEEP;

}

//*****************************************************************************
//*****************************************************************************
void  drv8833_leftForward(uint8_t dutyCycle)
{
		uint32_t d_val;
		d_val = (PWM_LOAD_VAL * (100-dutyCycle))/100;
		pwmConfig(PWM0_BASE,
		1,PWM_LOAD_VAL,d_val,d_val,
		PWM_CHANNEL_PWM,
		PWM_CHANNEL_LOW);
}

//*****************************************************************************
//*****************************************************************************
void  drv8833_leftReverse(uint8_t dutyCycle)
{

		uint32_t d_val;
		d_val = (PWM_LOAD_VAL * (100-dutyCycle))/100;
		pwmConfig(PWM0_BASE,
		1,PWM_LOAD_VAL,d_val,d_val,
		PWM_CHANNEL_LOW,
		PWM_CHANNEL_PWM);
}


//*****************************************************************************
//*****************************************************************************
void  drv8833_rightForward(uint8_t dutyCycle)
{
		uint32_t d_val;
		d_val = (PWM_LOAD_VAL * (100-dutyCycle))/100;
		pwmConfig(PWM1_BASE,
		1,PWM_LOAD_VAL,d_val,d_val,
		PWM_CHANNEL_PWM,
		PWM_CHANNEL_LOW);
}

//*****************************************************************************
//*****************************************************************************
void  drv8833_rightReverse(uint8_t dutyCycle)
{
		uint32_t d_val;
		d_val = (PWM_LOAD_VAL * (100-dutyCycle))/100;
		pwmConfig(PWM1_BASE,
		1,PWM_LOAD_VAL,d_val,d_val,
		PWM_CHANNEL_LOW,
		PWM_CHANNEL_PWM);
}

//*****************************************************************************
//*****************************************************************************
void  drv8833_turnLeft(uint8_t dutyCycle)
{
	drv8833_rightForward(dutyCycle);
	drv8833_leftReverse(dutyCycle);
}

//*****************************************************************************
//*****************************************************************************
void  drv8833_turnRight(uint8_t dutyCycle)
{
	drv8833_leftForward(dutyCycle);
	drv8833_rightReverse(dutyCycle);
}
