#include "interrupts.h"
extern volatile bool AlertSysTick;
extern volatile bool AlertUart7;

extern volatile int distanceToTravelLeft;
extern volatile int distanceToTravelRight;
extern volatile char uart[];

int uart_count;
char uart_char;


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
			uart_char = uartRxPoll(UART7_BASE,false);
			if(uart_char == 'R' || uart_char == '\r'){
				uart_count = 0;
			}
			else{
				uart[uart_count] = uart_char;
				uart_count = (uart_count + 1) % 3;
			}
			if(uart_count == 2){
					AlertUart7 = true;
					UART7->ICR = UART_ICR_RXIC; //clear the uart interrupt
			}
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
