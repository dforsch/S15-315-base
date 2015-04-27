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
#include "drv8833.h"
#include "ece315_lab3.h"
#include "lcd.h"

void setDistanceToTravelLeft(int);
void setDistanceToTravelRight(int);


//*****************************************************************************
// Global Variables
//*****************************************************************************
volatile bool AlertSysTick;
volatile bool AlertUart7;
extern volatile int distanceToTravelLeft;
extern volatile int distanceToTravelRight;
volatile char uart[4] = "R00"; // Place to put uart output
  
//*****************************************************************************
//*****************************************************************************
void initializeBoard(void)
{
  DisableInterrupts();
  serialDebugInit();
	ece315_lcdInit();
	rfInit();
	pw_config_gpio();
	SysTick_Config(2500);
	initializeADC(ADC0_BASE);
	drv8833_gpioInit();
	encodersInit();
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

	int seconds_elapsed = 0;
	char msg[80];

	wireless_com_status_t status;
	uint32_t data;
	distanceToTravelLeft = 0;
	distanceToTravelRight = 0;
	
	
  initializeBoard();
	
	ece315_lcdClear();


	
  uartTxPoll(UART0_BASE, "\n\r");
  uartTxPoll(UART0_BASE,"**************************************\n\r");
  uartTxPoll(UART0_BASE,"* ECE315 Default Project\n\r");
  uartTxPoll(UART0_BASE,"**************************************\n\r");
  // Infinite Loop
  while(true)
  {

		status = wireless_get_32(false, &data);
		if(status == NRF24L01_RX_SUCCESS)
		{
			memset(msg,0 ,80);
			sprintf(msg, "Data RXed: %c%c %d \r\n", data>>24, data>>16, data & 0xFFFF);
			//uartTxPoll(UART0_BASE,msg);
			if((data>>24) == 'F'){
				char buf[sizeof(int)*3+2];
				drv8833_rightForward(data & 0xFFFF);
				drv8833_leftForward(data & 0xFFFF);
			
				
         snprintf(buf, sizeof buf, "%lf", distanceToTravelLeft/75.13);
				
				ece315_lcdWriteString(1, "DIST: " );
			
				ece315_lcdWriteChar(1, buf[0], 50);
				ece315_lcdWriteChar(1, buf[1], 60);
				ece315_lcdWriteChar(1, buf[2], 70);
				ece315_lcdWriteChar(1, buf[3], 80);
				
				ece315_lcdWriteString(4, "DIR: FWD" );
				
			}
			else if((data >> 24) == 'L'){
				char buf[sizeof(int)*3+2];
				drv8833_turnLeft(data & 0xFFFF);
				snprintf(buf, sizeof buf, "%lf", distanceToTravelLeft/75.13);
				
				ece315_lcdWriteString(1, "DIST: " );
			
				ece315_lcdWriteChar(1, buf[0], 50);
				ece315_lcdWriteChar(1, buf[1], 60);
				ece315_lcdWriteChar(1, buf[2], 70);
				ece315_lcdWriteChar(1, buf[3], 80);
				
				ece315_lcdWriteString(4, "DIR: TURN" );
			}
			else if((data >> 24) == 'S'){
				drv8833_rightForward(1);
				drv8833_leftForward(1);
			}
			else{
				if(((data >> 16) & 0xFF) == 'V'){
					char buf[sizeof(int)*3+2];
						drv8833_rightReverse(data & 0xFFFF);
						drv8833_leftReverse(data & 0xFFFF);
					snprintf(buf, sizeof buf, "%lf", distanceToTravelLeft/75.13);
				
				ece315_lcdWriteString(1, "DIST: " );
			
				ece315_lcdWriteChar(1, buf[0], 50);
				ece315_lcdWriteChar(1, buf[1], 60);
				ece315_lcdWriteChar(1, buf[2], 70);
				ece315_lcdWriteChar(1, buf[3], 80);
				
				ece315_lcdWriteString(4, "DIR: REV" );
				}
				else{
					char buf[sizeof(int)*3+2];
					drv8833_turnRight(data & 0xFFFF);
							snprintf(buf, sizeof buf, "%lf", distanceToTravelLeft/75.13);
				
				ece315_lcdWriteString(1, "DIST: " );
			
				ece315_lcdWriteChar(1, buf[0], 50);
				ece315_lcdWriteChar(1, buf[1], 60);
				ece315_lcdWriteChar(1, buf[2], 70);
				ece315_lcdWriteChar(1, buf[3], 80);
				
				ece315_lcdWriteString(4, "DIR: TURN" );
				}
			}
		}
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
			
		}
		
		// Every second, 20000*50uS = 1s
		if(systick_count >= 20000){
			systick_count = 0;
			sprintf(output_string,"Pulse:%03d ADC:%03d UART:%s\n\r", pw_val, adc_val, uart);
			//uartTxPoll(UART0_BASE,output_string)
			seconds_elapsed++;

		}
		if(AlertUart7){
			// Every UART interrupt
			AlertUart7 = false;
			sprintf(output_string,"UART:%s\n\r",uart);
			uartTxPoll(UART0_BASE, output_string);
		}
		
		if(systick_count % 200 == 0){
			adc_val = getADCValue(ADC0_BASE, 0);
			// inches = V / (9.8mV/in)
			// V = adc_val / max_val * 5V
			adc_val = (adc_val+0.0)/(4095/5*.0098);
			sprintf(output_string,"Left: %d Right: %d\n\r", distanceToTravelLeft, distanceToTravelRight);
			//uartTxPoll(UART0_BASE,output_string);

		}
		
  }
}
