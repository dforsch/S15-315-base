#include "pwm.h"



//*****************************************************************************
//*****************************************************************************
uint8_t pwmConfig(
  uint32_t base,
  uint8_t pwm_generator, 
  uint32_t load, 
  uint32_t cmpa, 
  uint32_t cmpb,
  uint32_t gena,
  uint32_t genb
)
{
	uint32_t rcgc_mask = 0;
  uint32_t pr_mask = 0;
	PWM0_Type* PWM;
  
  // Verify that the base address and set rcgc_mask and pr_mask
   switch( base )
   {
     case PWM0_BASE:
     {
       // ADD CODE
       // Set rcgc_mask and pr_mask for GPIO Port A
       rcgc_mask = SYSCTL_RCGCPWM_R0;
			 pr_mask = SYSCTL_PRPWM_R0;
       break;
     }
     case PWM1_BASE:
     {
       // ADD CODE
       // Set rcgc_mask and pr_mask for GPIO Port B
       rcgc_mask = SYSCTL_RCGCPWM_R1;
			 pr_mask = SYSCTL_PRPWM_R1;
       break;
     }

     default:
     {
       return false;
     }
   }
	SYSCTL->RCGCPWM |= rcgc_mask;
	while((SYSCTL->PRPWM & pr_mask) == 0){
	}
	PWM = (PWM0_Type*) base;
	switch(pwm_generator){
		case 0:
			PWM->_0_CTL &= ~0x1;
			PWM->_0_CMPA = cmpa;
			PWM->_0_CMPB = cmpb;
			PWM->_0_GENA = gena;
			PWM->_0_GENB = genb;
			PWM->_0_LOAD = load;
			PWM->_0_CTL |= 1;
			PWM->ENABLE = PWM_ENABLE_PWM0A | PWM_ENABLE_PWM0B;

			break;
		case 1:
			PWM->_1_CTL &= ~0x1;
			PWM->_1_CMPA = cmpa;
			PWM->_1_CMPB = cmpb;
			PWM->_1_GENA = gena;
			PWM->_1_GENB = genb;
			PWM->_1_LOAD = load;
			PWM->_1_CTL |= 0x1;
			PWM->ENABLE = PWM_ENABLE_PWM1A | PWM_ENABLE_PWM1B;

			break;
		case 2:
			PWM->_2_CTL &= ~0x1;
			PWM->_2_CMPA = cmpa;
			PWM->_2_CMPB = cmpb;
			PWM->_2_GENA = gena;
			PWM->_2_GENB = genb;
			PWM->_2_LOAD = load;
			PWM->_2_CTL |= 1;
			PWM->ENABLE = PWM_ENABLE_PWM2A | PWM_ENABLE_PWM2B;
			break;
		case 3:
			PWM->_3_CTL &= ~0x1;
			PWM->_3_CMPA = cmpa;
			PWM->_3_CMPB = cmpb;
			PWM->_3_GENA = gena;
			PWM->_3_GENB = genb;
			PWM->_3_LOAD = load;
			PWM->_3_CTL |= 1;
			PWM->ENABLE = PWM_ENABLE_PWM3A | PWM_ENABLE_PWM3B;
			break;
		default:
			return false;
	}

  return 0;
}
