/* 	HD44780 LCD controler driver
 *
 * 	@autor 			Olexandr Karbivskyi
 *	@version 		1.0
 *  @ide 				Keil uVision 5
 *
 * 	Default pinout
 *  STM32F4DISCOVERY Board Description
 *
 *	VSS			GND				Ground 	
 * 	VDD			+5V				Positive Supply Voltage
 *	V0								Contrast Adjustment
 *	RS			PD1				Data\Command
 *  RW			GND				Read\Write
 *	E				PD0				Enable
 *	D0			none			
 *	D1			none
 * 	D2			none
 *	D3			none
 *	D4			PD4				
 *	D5			PD5
 *	D6			PD6
 *	D7			PD7
 *	A				+5V				Anode
 *	K				GND				Kathode
 *
 *
 *
 */
#ifndef LCD_H
#define LCD_H

#include "stm32f4xx.h"
#include "delay.h"

#define E_PIN          	GPIO_Pin_0
#define E_PORT         	GPIOD
#define E_RCC          	RCC_AHB1Periph_GPIOD


#define RS_PIN          GPIO_Pin_1
#define RS_PORT         GPIOD
#define RS_RCC          RCC_AHB1Periph_GPIOD


#define RW_PIN          GPIO_Pin_2
#define RW_PORT         GPIOD
#define RW_RCC          RCC_AHB1Periph_GPIOD


#define D7_PIN          GPIO_Pin_7
#define D7_PORT         GPIOD
#define D7_RCC          RCC_AHB1Periph_GPIOD


#define D6_PIN          GPIO_Pin_6
#define D6_PORT         GPIOD
#define D6_RCC          RCC_AHB1Periph_GPIOD


#define D5_PIN          GPIO_Pin_5
#define D5_PORT         GPIOD
#define D5_RCC          RCC_AHB1Periph_GPIOD


#define D4_PIN          GPIO_Pin_4
#define D4_PORT         GPIOD
#define D4_RCC          RCC_AHB1Periph_GPIOD


#define E_SET           GPIO_SetBits(E_PORT, E_PIN)
#define E_RESET         GPIO_ResetBits(E_PORT, E_PIN)
#define RS_SET          GPIO_SetBits(RS_PORT, RS_PIN)
#define RS_RESET        GPIO_ResetBits(RS_PORT, RS_PIN)
#define D7_SET          GPIO_SetBits(D7_PORT, D7_PIN)
#define D7_RESET        GPIO_ResetBits(D7_PORT, D7_PIN)
#define D6_SET          GPIO_SetBits(D6_PORT, D6_PIN)
#define D6_RESET        GPIO_ResetBits(D6_PORT, D6_PIN)
#define D5_SET          GPIO_SetBits(D5_PORT, D5_PIN)
#define D5_RESET        GPIO_ResetBits(D5_PORT, D5_PIN)
#define D4_SET          GPIO_SetBits(D4_PORT, D4_PIN)
#define D4_RESET        GPIO_ResetBits(D4_PORT, D4_PIN)

/*#define HD44780_RETURN_HOME				0x01
#define HD44780_CLEAR_DISPLAY			0x02
#define HD44780_INITIAL						0x30


#define HD44780_EMS_INCREMENT			0x06
#define HD44780_EMS_DECREMENT			0x04
#define HD44780_EMS_SHIFTS_ON			0x05
#define HD44780_EMS_SHIFTS_OFF		0x04


#define HD44780_DC_DISPLAY_ON			0x0C
#define HD44780_DC_DISPLAY_OFF		0x08
#define HD44780_DC_CURSOR_ON			0x0A
#define HD44780_DC_CURSOR_OFF			0x08
#define HD44780_DC_BLINKING_ON		0x09
#define HD44780_DC_BLINKING_OFF		0x08

#define HD44780_FS_4BIT						0x20
#define HD44780_FS_8BIT						0x30
#define HD44780_FS_2LINES					0x28
#define HD44780_FS_1LINES					0x20
#define HD44780_5_10_DOTS					0x24
#define HD44780_5_8_DOTS					0x20
*/

/* Functions Prototypes */

void HD44780_GPIO_Init(void);
void LCD_Delay(void);
void HD44780_Init (void);
void HD44780_Set_Data(uint8_t data);
void HD44780_Send_Data(uint8_t data);
void HD44780_Send_Command(uint8_t data);
void HD44780_Send_String(uint8_t line, char* str);
void HD44780_Cursor_On(void);
void HD44780_Cursor_Off(void);
void HD44780_Display_On(void);
void HD44780_Display_Off(void);
void HD44780_Display_Clear(void);

void HD44780_Send_Char(char c);
void HD44780_Set_Cursor(uint8_t y, uint8_t x);


#endif
