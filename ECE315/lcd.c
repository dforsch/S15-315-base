#include "lcd.h"
#include "../include/gpioPort.h"
#include "../include/spi.h"
#include "../include/fonts.h"


	uint8_t dataIn[2];
	uint8_t dataOut[2];

//*****************************************************************************
// Initializes the pins needed to communicate with the LCD and issues the 
// initialization sequence found in the LCD datasheet via the SPI interface
//*****************************************************************************
void ece315_lcdInit(void)
{
  
SPI_CONFIG LCD_config = {
    LCD_SPI_BASE,
    DISABLED,                   //EndOfTransEn
    SSI_MASTER_EN,              //MasSlvSelect
    SSI_EN,                     //SSIEnable
    DISABLED,                   //LoopbackModeEn
    10,                         //ClkPrescaleDiv
    SSI_SPH_SECOND,             //SerialCPHA
    SSI_SPO_HIGH,               //SerialCPOL
    SSI_CR0_FRF_MOTO,           //FrameFormatSelect
    SSI_CR0_DSS_8,              //DataSizeSelect
    DISABLED,                   //TxFifoIntMask
    DISABLED,                   //RxFifoIntMask
    DISABLED,                   //RxTimeoutIntMask
    DISABLED,                   //RxOverrunIntMask
    DISABLED,                   //TxDMAEn
    DISABLED,                   //RxDMAEn
  };
  
	gpio_enable_port(LCD_GPIO_BASE);

  // Configure SPI CLK
  gpio_config_digital_enable(LCD_GPIO_BASE, LCD_CLK_PIN);
  gpio_config_alternate_function(LCD_GPIO_BASE, LCD_CLK_PIN);
  gpio_config_port_control(LCD_GPIO_BASE, LCD_CLK_PIN_PCTL);
    
  // Configure SPI CS
  gpio_config_digital_enable(LCD_GPIO_BASE, LCD_CS_PIN);
	//gpio_config_enable_output(LCD_GPIO_BASE, LCD_CS_PIN);
  gpio_config_alternate_function(LCD_GPIO_BASE, LCD_CS_PIN);
  gpio_config_port_control(LCD_GPIO_BASE, LCD_CS_PIN_PCTL);

  // Configure SPI MOSI
  gpio_config_digital_enable(LCD_GPIO_BASE, LCD_MOSI_PIN);
  gpio_config_alternate_function(LCD_GPIO_BASE, LCD_MOSI_PIN);
  gpio_config_port_control(LCD_GPIO_BASE, LCD_MOSI_PIN_PCTL);
  
  // Configure CD
  gpio_config_digital_enable(GPIO_LCD_CD_BASE,LCD_CD_PIN);
  gpio_config_enable_output(GPIO_LCD_CD_BASE,LCD_CD_PIN);
  
  // Configure RST_N
  gpio_config_digital_enable(GPIO_LCD_RST_N_BASE, LCD_RST_N_PIN);
  gpio_config_enable_output(GPIO_LCD_RST_N_BASE, LCD_RST_N_PIN);
  
	// Bring the LCD out of reset
	GPIOD->DATA |= LCD_RST_N_PIN;
  InitializeSPI(&LCD_config);
  
  GPIOD->DATA &= ~LCD_CD_PIN;
	GPIOD->DATA |= LCD_CS_PIN;
  // Use spiTx() from the ece315 driver library to issue the sequence of 
  // commands in the LCD data sheet to initialize the LCD.
	GPIOD->DATA &= ~LCD_CS_PIN;
  dataIn[0] = 0x40;  
  spiTx(SSI3_BASE, dataIn, 1, dataOut);
	dataIn[0] = 0xA1;  
  spiTx(SSI3_BASE, dataIn, 1, dataOut);
	dataIn[0] = 0xC0;  
  spiTx(SSI3_BASE, dataIn, 1, dataOut);
	dataIn[0] = 0xA4;  
  spiTx(SSI3_BASE, dataIn, 1, dataOut);
	dataIn[0] = 0xA6;  
  spiTx(SSI3_BASE, dataIn, 1, dataOut);
	dataIn[0] = 0xA2;  
  spiTx(SSI3_BASE, dataIn, 1, dataOut);
	dataIn[0] = 0x2F;  
  spiTx(SSI3_BASE, dataIn, 1, dataOut);
	dataIn[0] = 0x27;  
  spiTx(SSI3_BASE, dataIn, 1, dataOut);
	dataIn[0] = 0x81;  
  spiTx(SSI3_BASE, dataIn, 1, dataOut);
	dataIn[0] = 0x10;  
  spiTx(SSI3_BASE, dataIn, 1, dataOut);
	dataIn[0] = 0xFA;  
  spiTx(SSI3_BASE, dataIn, 1, dataOut);
	dataIn[0] = 0x90;  
  spiTx(SSI3_BASE, dataIn, 1, dataOut);
	dataIn[0] = 0xAF;  
  spiTx(SSI3_BASE, dataIn, 1, dataOut);
	GPIOD->DATA |= LCD_CS_PIN;
	GPIOD->DATA |= LCD_CD_PIN;
}

 //****************************************************************************
// Sets the currently active page
//*****************************************************************************
  void ece315_lcdSetPage(uint8_t   page)
  {
		  GPIOD->DATA &= ~LCD_CD_PIN;
			dataIn[0] = 0xB0 | (page & 0x0F);
			spiTx(LCD_SPI_BASE, dataIn, 1, dataOut);
		  GPIOD->DATA |= LCD_CD_PIN;
  }
  
//*****************************************************************************
// Sets the currently active column
//*****************************************************************************
void ece315_lcdSetColumn(uint8_t   column)
{
      GPIOD->DATA &= ~LCD_CD_PIN;
			dataIn[0] = 0x00 | (column & 0x0F);
			spiTx(LCD_SPI_BASE, dataIn, 1, dataOut);
			dataIn[0] = 0x10 | (column >> 4);
			spiTx(LCD_SPI_BASE, dataIn, 1, dataOut);
			GPIOD->DATA |= LCD_CD_PIN;  
}
  
//*****************************************************************************
// Writes 8-bits of data to the current column of the LCD
//*****************************************************************************
  void ece315_lcdWriteData(uint8_t   data)
  {
			dataIn[0] = data;
			spiTx(LCD_SPI_BASE, dataIn, 1, dataOut);   
  }
  
//*****************************************************************************
// Erases the LCD screen.
//*****************************************************************************
 void ece315_lcdClear(void)
 {
	 int i,j;
	 for(i=0;i<8;i++){
		 ece315_lcdSetPage(i);
		 for(j=0;j<102;j++){
			 ece315_lcdSetColumn(j);
			 ece315_lcdWriteData(0x00);
		 }
	 }
 }

//*****************************************************************************
// Each character is 10 columns wide.  The colStart is the column number where
// the first column will be printed.
//
// font.c contains a lookup table for printing out characters to the LCD screen.
// You should note that each character is 16 pixels high and 10 pixels wide.
// For each character, you will have to write to two different pages to print
// out a single character.  
//*****************************************************************************
void ece315_lcdWriteChar( uint8_t page, char c, uint8_t colStart)
 {
	 uint8_t data;
	 int i;
	 ece315_lcdSetPage(page);
	 c = c - ' ';
	 for(i=0; i<10; i++){
		data = courierNew_10ptBitmaps[c*20+i];
		ece315_lcdSetColumn(colStart+i);
		ece315_lcdWriteData(data);
	 }
	 ece315_lcdSetPage(page+1);
	 for(i=10; i<20; i++){
		data = courierNew_10ptBitmaps[c*20+i];
		ece315_lcdSetColumn(colStart+i-10);
		ece315_lcdWriteData(data);
	 }
	 
 }
 
//*****************************************************************************
// Write a string of characters out to the LCD screen.  Only the first 
// 10 characters will be printed.  The function will also terminate when
// a null character is encountered.
//*****************************************************************************
void ece315_lcdWriteString( uint8_t line, char *string)
{
  int i;
	for(i=0; (i<10)&&(string[i] != 0); i++){
			ece315_lcdWriteChar(line, string[i], i*10);
	}
}  

