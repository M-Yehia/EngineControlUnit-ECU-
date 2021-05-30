/*
 * Pulses_Counter.c
 *
 *  Created on: 5 May 2021
 *      Author: OMAR
 */

#include "common_macros.h"
#include "Counter.h"
#include "lcd.h"
#include "std_types.h"
#include "micro_config.h"

int main(void)
{
	unsigned char Cnt = 0;
	char arr[10];
	unsigned char arr1[16];

	Counter_init(0);

	/* Configure PA0 as Input */
	CLEAR_BIT(DDRA,PA0);


	LCD_Init();
	LCD_Clear_Display();
	LCD_Send_String_Row_Column(0,0,"The Counts : ");



	while(1)
	{
		if(BIT_SET(PINA,PA0))
		{
			Cnt = Counter_GetValue();
			LCD_GoTO_Row_Colunmn(1,0);
			LCD_IntToString(Cnt,arr);
			LCD_Send_String(arr);
			LCD_Send_String(" Counts");
		}
	}

	return 0;
}
