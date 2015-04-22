#include "lcd.h"
#include "../include/spi.h"


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
  

  // Configure SPI CLK
  gpioConfigDigitalEnable(LCD_GPIO_BASE, LCD_CLK_PIN);
  gpioConfigAltFunction(LCD_GPIO_BASE, LCD_CLK_PIN);
  gpioConfigPortConfig(LCD_GPIO_BASE, LCD_CLK_PIN_PCTL);
    
  // Configure SPI CS
  gpioConfigDigitalEnable(LCD_GPIO_BASE, LCD_CS_PIN);
  gpioConfigAltFunction(LCD_GPIO_BASE, LCD_CS_PIN);
  gpioConfigPortConfig(LCD_GPIO_BASE, LCD_CS_PIN_PCTL);

  // Configure SPI MOSI
  gpioConfigDigitalEnable(LCD_GPIO_BASE, LCD_MOSI_PIN);
  gpioConfigAltFunction(LCD_GPIO_BASE, LCD_MOSI_PIN);
  gpioConfigPortConfig(LCD_GPIO_BASE, LCD_MOSI_PIN_PCTL);
  
  // Configure CD
  gpioConfigDigitalEnable(GPIO_LCD_CD_BASE,LCD_CD_PIN);
  gpioConfigPinAsOutput(GPIO_LCD_CD_BASE,LCD_CD_PIN);
  
  // Configure RST_N
  gpioConfigDigitalEnable(GPIO_LCD_RST_N_BASE, LCD_RST_N_PIN);
  gpioConfigPinAsOutput(GPIO_LCD_RST_N_BASE, LCD_RST_N_PIN);
  
  InitializeSPI(&LCD_config);
  
  
  // Bring the LCD out of reset
  
  // Use spiTx() from the ece315 driver library to issue the sequence of 
  // commands in the LCD data sheet to initialize the LCD.  
  
  
}

 //****************************************************************************
// Sets the currently active page
//*****************************************************************************
  void ece315_lcdSetPage(uint8_t   page)
  {
    
  }
  
//*****************************************************************************
// Sets the currently active column
//*****************************************************************************
void ece315_lcdSetColumn(uint8_t   column)
{
  
}
  
//*****************************************************************************
// Writes 8-bits of data to the current column of the LCD
//*****************************************************************************
  void ece315_lcdWriteData(uint8_t   data)
  {
    
  }
  
//*****************************************************************************
// Erases the LCD screen.
//*****************************************************************************
 void ece315_lcdClear(void)
 {
   
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
   
 }
 
//*****************************************************************************
// Write a string of characters out to the LCD screen.  Only the first 
// 10 characters will be printed.  The function will also terminate when
// a null character is encountered.
//*****************************************************************************
void ece315_lcdWriteString( uint8_t line, char *string)
{
  
}  

