/*	HD44780 lCD controler driver
 *	@author CAJI0
 *	
 *
 *
 *
 *
 */

#include "hd44780.h"

/* Commands*/
#define HD44780_CLEAR_DISPLAY         0x01
#define HD44780_RETURN_HOME           0x02
#define HD44780_ENTRYMODESET          0x04
#define HD44780_DISPLAYCONTROL        0x08
#define HD44780_CURSORSHIFT           0x10
#define HD44780_FUNCTIONSET           0x20
#define HD44780_SETCGRAMADDR          0x40
#define HD44780_SETDDRAMADDR          0x80

/* Flags for display entry mode */
#define HD44780_ENTRYRIGHT            0x00
#define HD44780_ENTRYLEFT             0x02
#define HD44780_ENTRYSHIFTINCREMENT   0x01
#define HD44780_ENTRYSHIFTDECREMENT   0x00

/* Flags for display on/off control */
#define HD44780_DISPLAY_ON            0x04
#define HD44780_CURSOR_ON             0x02
#define HD44780_BLINK_ON              0x01

/* Flags for display/cursor shift */
#define HD44780_DISPLAY_MOVE          0x08
#define HD44780_CURSOR_MOVE           0x00
#define HD44780_MOVE_RIGHT            0x04
#define HD44780_MOVE_LEFT             0x00

/* Flags for function set */
#define HD44780_8BITMODE              0x10
#define HD44780_4BITMODE              0x00
#define HD44780_2LINE                 0x08
#define HD44780_1LINE                 0x00
#define HD44780_5x10DOTS              0x04
#define HD44780_5x8DOTS               0x00


typedef struct {
	uint8_t  HD44780_Control;
	uint8_t  HD44780_Function;
	uint8_t  HD44780_Mode;
	uint8_t  HD44780_X;
	uint8_t  HD44780_Y;
} HD44780_Options_t;


/* Private functions */
void LCD_Delay();
void Enable_Strobe(void);

/* Private variable */
static HD44780_Options_t HD44780_Opts;


/* Function for GPIO initialization */
void HD44780_GPIO_Init (void) {

  GPIO_InitTypeDef GPIO_InitStruct;
  /* Enable  clock for GPIO ports */
  RCC_AHB1PeriphClockCmd (E_RCC|RS_RCC|D7_RCC|D6_RCC|D5_RCC|D4_RCC, ENABLE);

	/* Common setting for all pins */
  GPIO_InitStruct.GPIO_Mode = 	GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = 	GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	/* Enable Pin */
	GPIO_InitStruct.GPIO_Pin = 	E_PIN;
  GPIO_Init(E_PORT, &GPIO_InitStruct);

	/* RS Pin */
	GPIO_InitStruct.GPIO_Pin = 	RS_PIN;
  GPIO_Init(RS_PORT, &GPIO_InitStruct);

	/* D7 Pin */
  GPIO_InitStruct.GPIO_Pin = 	D7_PIN;
  GPIO_Init(D7_PORT, &GPIO_InitStruct);

	/* D6 Pin */
  GPIO_InitStruct.GPIO_Pin = 	D6_PIN;
  GPIO_Init(D6_PORT, &GPIO_InitStruct);

	/* D5 Pin */
  GPIO_InitStruct.GPIO_Pin = 	D5_PIN;
  GPIO_Init(D5_PORT, &GPIO_InitStruct);

	/* D4 Pin */
  GPIO_InitStruct.GPIO_Pin = 	D4_PIN;
  GPIO_Init(D4_PORT, &GPIO_InitStruct);

  E_RESET;
  RS_RESET;
  D7_RESET;
  D6_RESET;
  D5_RESET;
  D4_RESET;
}

void LCD_Delay (void){
  int i = 100000;
  while(i--);
}


/* Set LCD data for 4-bit mode */
void HD44780_Set_Data (uint8_t data){
  if(data & 0x08) {D7_SET;} else {D7_RESET;}
  if(data & 0x04) {D6_SET;} else {D6_RESET;}
  if(data & 0x02) {D5_SET;} else {D5_RESET;}
  if(data & 0x01) {D4_SET;} else {D4_RESET;}
	Enable_Strobe();
}
/* Function created strob for enable signal*/
void Enable_Strobe(void)
{
	E_SET;
	User_Delay_us(20);
  E_RESET;
	User_Delay_us(40);
}

/* Send data to HD44780 controller*/
void HD44780_Send_Data(uint8_t data){
  RS_SET;

	/* set high nibble */
  HD44780_Set_Data((data >> 4));
	/* set low nibble */
  HD44780_Set_Data(data & 0x0F);
}

/* Send command to HD44780 controler */
void HD44780_Send_Command(uint8_t data){
  RS_RESET;

	/* set high nibble */
  HD44780_Set_Data((data >> 4));
	/* set low nibble */
  HD44780_Set_Data(data & 0x0F);
}

/* Initial initialization of HD44780 controller */
void HD44780_Init (void){
  /* GPIO initialization */
  HD44780_GPIO_Init();

  /* wait at least 40 ms */
	User_Delay_us(45000);

  /* first time function set */
	HD44780_Set_Data(0x03);
	User_Delay_us(4500);

  /* second time function set */
  HD44780_Set_Data(0x03);
	User_Delay_us(4500);

  /* third time function set */
  HD44780_Set_Data(0x03);
	User_Delay_us(4500);

  /* Set 4 bit interface mode */
	HD44780_Set_Data(0x02);
	User_Delay_us(100);

  /* Functional set: 4 bit interface mode, 2 lines, font size 5x8 dots */
  HD44780_Opts.HD44780_Function = HD44780_4BITMODE | HD44780_5x8DOTS | HD44780_2LINE;
	HD44780_Send_Command(HD44780_FUNCTIONSET | HD44780_Opts.HD44780_Function);


  /* Display on, cursor off, blinking off */
  HD44780_Opts.HD44780_Control = HD44780_DISPLAY_ON;
  HD44780_Send_Command(HD44780_DISPLAYCONTROL | HD44780_Opts.HD44780_Control);

	HD44780_Display_Clear();

  /* Entry mode set:  */
	HD44780_Opts.HD44780_Mode = HD44780_ENTRYLEFT | HD44780_ENTRYSHIFTDECREMENT;
	HD44780_Send_Command(HD44780_ENTRYMODESET | HD44780_Opts.HD44780_Mode);

	HD44780_Send_Command(HD44780_RETURN_HOME);
  User_Delay_us(2000);



}

/* Function for send string to HD44780 controller at start of needed line */
void HD44780_Send_String(uint8_t line, char* str){

  uint8_t i = 0;

  if(!line)
	{
		HD44780_Send_Command(0x80);
	}
	else
	{
		HD44780_Send_Command(0xC0);
	}
	User_Delay_ms(1);

  while(*str)
	{
		HD44780_Send_Data(*str++);
	}

}
void HD44780_Send_Char(char c){
	HD44780_Send_Data(c);
}


void HD44780_Set_Cursor(uint8_t y, uint8_t x){
	if(y)
		HD44780_Send_Command(0xC0|x%16);
	else
		HD44780_Send_Command(0x80|x%16);
	User_Delay_us(40);
}

void HD44780_Cursor_On(void){
  HD44780_Opts.HD44780_Control |= HD44780_CURSOR_ON;
  HD44780_Send_Command(HD44780_DISPLAYCONTROL | HD44780_Opts.HD44780_Control);
}

void HD44780_Cursor_Off(void){
  HD44780_Opts.HD44780_Control &= ~HD44780_CURSOR_ON;
  HD44780_Send_Command(HD44780_DISPLAYCONTROL | HD44780_Opts.HD44780_Control);
}

void HD44780_Display_On(void){
  HD44780_Opts.HD44780_Control |= HD44780_DISPLAY_ON;
  HD44780_Send_Command(HD44780_DISPLAYCONTROL | HD44780_Opts.HD44780_Control);
}

void HD44780_Display_Off(void){
  HD44780_Opts.HD44780_Control &= ~HD44780_DISPLAY_ON;
  HD44780_Send_Command(HD44780_DISPLAYCONTROL | HD44780_Opts.HD44780_Control);
}

void HD44780_Display_Clear(void){
  HD44780_Send_Command(HD44780_CLEAR_DISPLAY);
	User_Delay_us(4000);
}
