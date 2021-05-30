/*
 * LCD.h
 *
 *  Created on: 5 May 2021
 *      Author: OMAR
 */

#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/* UnComment the needed CFG */
//#define LCD_DATA_HIGH_NIBBLE
#define LCD_DATA_LOW_NIBBLE

#define LCD_BIT_MODE 4

#define RS PC6
#define E  PC7

#define LCD_CTRL_PORT_RS PORTC
#define LCD_CTRL_PORT_DIR_RS DDRC

#define LCD_CTRL_PORT_E PORTC
#define LCD_CTRL_PORT_DIR_E DDRC

#define LCD_DATA_PORT PORTB
#define LCD_DATA_PORT_DIR DDRB

#define _8BIT_2LINE (0x38)
#define _4BIT_2LINE (0x28)
#define DISPLAY_ON_CURSOR_ON (0x0E)
#define DISPLAY_ON_CURSOR_OFF (0x0C)
#define CLEAR_DISPLAY (0x01)
#define SET_CURSOR_LOCATION (0x80)
#define ReturnHome (0x02)

void LCD_Init(void);
void LCD_Clear_Display(void);
void LCD_Send_Command(unsigned char command);
void LCD_Send_character(char character);
void LCD_Send_String(char *string);
void LCD_GoTO_Row_Colunmn(char Row,char Column);
void LCD_Send_String_Row_Column(char Row,char Column,char *String);
void LCD_IntToString(int integer,char *String);
void LCD_Set_Custom_Char(unsigned char Location ,unsigned char * Pattern);



#endif /* LCD_H_ */
