#include "interrupts.h"
extern volatile bool AlertSysTick;
extern volatile bool AlertUart7;

extern volatile int distanceToTravelLeft;
extern volatile int distanceToTravelRight;


// SysTick Handler
void SysTick_Handler(void)
{
   uint32_t val;

   // Alert the main application the SysTick Timer has expired
   AlertSysTick = true;

   // Clears the SysTick Interrupt
   val = SysTick->VAL;
}


void UART7_Handler(void)
{
	AlertUart7 = true;
	UART7->ICR = UART_ICR_RXIC; //clear the uart interrupt
}

void GPIOF_Handler(void)
{
	distanceToTravelLeft--;
	GPIOF->ICR = 0xFF;
}

void GPIOC_Handler(void)
{
	distanceToTravelRight--;
	GPIOC->ICR = 0xFF;
}
