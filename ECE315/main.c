// Copyright (c) 2014, Joe Krachey
// All rights reserved.
//
// Redistribution and use in binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>


#include "TM4C123.h"
#include "boardUtil.h"



//*****************************************************************************
// Global Variables
//*****************************************************************************
volatile bool AlertSysTick;
volatile bool AlertUart7;
  
//*****************************************************************************
//*****************************************************************************
void initializeBoard(void)
{
  DisableInterrupts();
  serialDebugInit();
	pw_config_gpio();
	SysTick_Config(2500);
	initializeADC(ADC0_BASE);
  EnableInterrupts();
}


//*****************************************************************************
//*****************************************************************************
int 
main(void)
{

  int count;
	int pw_val;
	int systick_count = 0;
	bool pe2;
	bool pe2_prev;
	char output_string[50];
	int adc_val; //value read by adc
	char uart[3]; // Place to put uart output
	int uart_count;
	char uart_char;
	
	
	
  initializeBoard();

  uartTxPoll(UART0_BASE, "\n\r");
  uartTxPoll(UART0_BASE,"**************************************\n\r");
  uartTxPoll(UART0_BASE,"* ECE315 Default Project\n\r");
  uartTxPoll(UART0_BASE,"**************************************\n\r");
  
  // Infinite Loop
  while(1)
  {
		// Every 50uS
		if(AlertSysTick){
			pe2_prev = pe2;
			pe2 = GPIOE->DATA & PE2;
			if(pe2 && !pe2_prev){
				count = 0;
			}
			count++;
			if(!pe2 && pe2_prev){
				pw_val = count*50/147.;
			}
			AlertSysTick = false;
			systick_count++;
			GPIOE->DATA ^= PE5;
		}
		
		// Every second, 20000*50uS = 1s
		if(systick_count > 20000){
			systick_count = 0;
			sprintf(output_string,"Pulse:%03d ADC:%03d UART:%s\n\r", pw_val, adc_val, uart);
			uartTxPoll(UART0_BASE,output_string);
		}
		
		if(systick_count % 200 == 0){
			adc_val = getADCValue(ADC0_BASE, 0);
			// inches = V / (9.8mV/in)
			// V = adc_val / max_val * 5V
			adc_val = (adc_val+0.0)/(4095/5*.0098);
			GPIOE->DATA ^= PE4;
		}
		
		// Every UART interrupt
		if(AlertUart7)
		{
			AlertUart7 = false;
			uart_char = uartRxPoll(UART7_BASE,true);
			if(uart_char == 'R' || uart_char == '\r'){
				uart_count = 0;
			}
			else{
				uart[uart_count] = uart_char;
				uart_count = (uart_count + 1) % 3;
			}
		}
		
  }
}
	
