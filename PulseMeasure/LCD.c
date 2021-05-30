/*
 * LCD.c
 *
 *  Created on: 5 May 2021
 *      Author: OMAR
 */

#include "LCD.h"

/*LCD initialization & clearing*/
void LCD_Init(void)
{
	LCD_CTRL_PORT_DIR_RS |=(1<<RS);
	LCD_CTRL_PORT_DIR_E  |=(1<<E);

#if LCD_BIT_MODE  == 4

#ifdef LCD_DATA_HIGH_NIBBLE
	LCD_DATA_PORT_DIR |=0xF0;
	LCD_Send_Command(ReturnHome);
	LCD_Send_Command(_4BIT_2LINE);
#endif

#ifdef LCD_DATA_LOW_NIBBLE
	LCD_DATA_PORT_DIR |=0x0F;
	LCD_Send_Command(ReturnHome);
	LCD_Send_Command(_4BIT_2LINE);
#endif

#elif LCD_BIT_MODE == 8
	LCD_DATA_PORT_DIR=0xff;
	LCD_Send_Command(ReturnHome);
	LCD_Send_Command(_8BIT_2LINE);
#endif
	_delay_ms(1);
	LCD_Send_Command(CLEAR_DISPLAY);
	LCD_Send_Command(DISPLAY_ON_CURSOR_OFF);
}

/*Sending A command to the LCD */
void LCD_Send_Command(unsigned char command)
{
	/*LCD as 8bit mode*/
#if LCD_BIT_MODE == 8
	LCD_CTRL_PORT_RS &=~(1<<RS); // command mode
	_delay_ms(1);
	LCD_CTRL_PORT_E |=(1<<E);
	_delay_ms(1);
	LCD_DATA_PORT =command;
	LCD_CTRL_PORT_E &=~(1<<E);
	_delay_ms(1);

	/*LCD as 8bit mode*/
#elif LCD_BIT_MODE == 4
#ifdef   LCD_DATA_HIGH_NIBBLE
	LCD_CTRL_PORT_RS &=~(1<<RS); // command mode
	_delay_ms(1);
	LCD_CTRL_PORT_E |=(1<<E);
	_delay_ms(1);
	/* Clearing Data Port  For new Data*/
	LCD_DATA_PORT &= (0x0F);
	LCD_DATA_PORT |=(command&0xF0);
	LCD_CTRL_PORT_E &=~(1<<E);
	_delay_ms(1);

	LCD_CTRL_PORT_RS &=~(1<<RS); // command mode
	LCD_CTRL_PORT_E |=(1<<E);
	_delay_ms(1);
	/* Clearing Data Port  For new Data*/
	LCD_DATA_PORT &= (0x0F);
	LCD_DATA_PORT |=( (command<<4)&0xF0);
	LCD_CTRL_PORT_E &=~(1<<E);
	_delay_ms(1);
#endif

#ifdef LCD_DATA_LOW_NIBBLE
	LCD_CTRL_PORT_RS &=~(1<<RS); // command mode
	_delay_ms(1);
	LCD_CTRL_PORT_E |=(1<<E);
	_delay_ms(1);
	/* Clearing Data Port  For new Data*/
	LCD_DATA_PORT &= (0xF0);
	LCD_DATA_PORT =((command>>4)&0x0F);
	LCD_CTRL_PORT_E &=~(1<<E);
	_delay_ms(1);

	LCD_CTRL_PORT_RS &=~(1<<RS); // command mode
	LCD_CTRL_PORT_E |=(1<<E);
	_delay_ms(1);
	/* Clearing Data Port  For new Data*/
	LCD_DATA_PORT &= (0xF0);
	LCD_DATA_PORT |=(command&0x0F);
	LCD_CTRL_PORT_E &=~(1<<E);
	_delay_ms(1);
#endif

#endif

}
/*LCD Clearing the display*/
void LCD_Clear_Display(void)
{
	LCD_Send_Command(CLEAR_DISPLAY);
}

/*LCD displaying a character*/
void LCD_Send_character(char character)
{
	/*LCD as 8bit mode*/
#if LCD_BIT_MODE == 8
	LCD_CTRL_PORT_RS |=(1<<RS);// Data mode
	LCD_CTRL_PORT_E |=(1<<E);
	_delay_ms(1);
	LCD_DATA_PORT =character;
	_delay_ms(1);
	LCD_CTRL_PORT_E &=~(1<<E);
	_delay_ms(1);

	/*LCD as 8bit mode*/
#elif LCD_BIT_MODE == 4
#ifdef LCD_DATA_HIGH_NIBBLE

	LCD_CTRL_PORT_RS |=(1<<RS); // Data mode
	_delay_ms(1);
	LCD_CTRL_PORT_E |=(1<<E);
	_delay_ms(1);

	/* Clearing Data Port  For new Data*/
	LCD_DATA_PORT &= (0x0F);
	LCD_DATA_PORT |=((character)&0xF0);
	LCD_CTRL_PORT_E &=~(1<<E);
	_delay_ms(1);

	LCD_CTRL_PORT_RS |=(1<<RS); // Data mode
	LCD_CTRL_PORT_E |=(1<<E);
	_delay_ms(1);
	/* Clearing Data Port  For new Data*/
	LCD_DATA_PORT &= (0x0F);
	LCD_DATA_PORT |=( (character<<4)&0xF0);
	LCD_CTRL_PORT_E &=~(1<<E);
	_delay_ms(1);
#endif

#ifdef LCD_DATA_LOW_NIBBLE

	LCD_CTRL_PORT_RS |=(1<<RS); // Data mode
	_delay_ms(1);
	LCD_CTRL_PORT_E |=(1<<E);
	_delay_ms(1);

	/* Clearing Data Port  For new Data*/
	LCD_DATA_PORT &= (0xF0);
	LCD_DATA_PORT =((character >> 4) & 0x0F);
	LCD_CTRL_PORT_E &=~(1<<E);
	_delay_ms(1);

	LCD_CTRL_PORT_RS |=(1<<RS); // Data mode
	LCD_CTRL_PORT_E |=(1<<E);
	_delay_ms(1);
	/* Clearing Data Port  For new Data*/
	LCD_DATA_PORT &= (0xF0);
	LCD_DATA_PORT =(character & 0x0F);
	LCD_CTRL_PORT_E &=~(1<<E);
	_delay_ms(1);
#endif
#endif
}

/*LCD displaying a String*/
void LCD_Send_String(char *string)
{
	int i=0;
	while(string[i]!='\0')
	{
		LCD_Send_character(string[i]);
		i++;
	}
}

/*set the cursor at certain location*/
void LCD_GoTO_Row_Colunmn(char Row,char Column)
{
	unsigned char address=0;

	switch (Row)
	{
	case 0:address =(Column);break;
	case 1:address =(Column+0x40);break;
	case 2:address =(Column+0x10);break;
	case 3:address =(Column+0x50);break;
	}

	/* we need to apply the corresponding command 0b10000000+Address */
	LCD_Send_Command(address|(SET_CURSOR_LOCATION));

}

/*lcd Send String at certain location*/
void LCD_Send_String_Row_Column(char Row,char Column,char *String)
{
	LCD_GoTO_Row_Colunmn(Row,Column);
	LCD_Send_String(String);
}

/*since the lcd is alpha Numeric print characters only*/
void LCD_IntToString(int integer,char *String)
{
	itoa(integer,String,10);
}


/***********************************************************************
 * Function    :LCD_Set_Custom_Char(unsigned char Location , unsigned char * Pattern);
 *
 * DESCRIPTION : This function used to upload a custom char which user wants to map
 * 				it using CGRAM (Character Generator Ram Address set).
 *
 * PARAMETERS  : location : to define which CGRAM ADD offset. It takes from (0 to 7)
 * 						   as CGRAM has 64bytes and LCD as 5x8 dots in function set then you
 * 						   can define a total of 8 user defined patterns (1 Byte for each row
 * 						   and 8 rows for each pattern).
 * 				Pattern: pointer to 8 pointer to char as CGRAM needs 8 char to map
 *
 * Return Value: Void.
 ***********************************************************************/

void LCD_Set_Custom_Char(unsigned char Location , unsigned char * Pattern)
{
	unsigned char ArrayIndex =0;
	/* the Add offset must be 0 to 7*/
	if (Location < 8)
	{
		LCD_Send_Command(0x40 + (Location * 8));
		for (ArrayIndex = 0; ArrayIndex < 8; ArrayIndex++)
		{
			LCD_Send_character(Pattern[ArrayIndex]);
		}
	}
	else
	{
		/* TODO: return Error */
	}
}
