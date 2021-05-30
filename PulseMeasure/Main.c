/*
 * Main.c
 *
 *  Created on: 5 May 2021
 *      Author: OMAR
 */

#include "LCD.h"
#include "Icu.h"
#include "std_types.h"

volatile float DutyCycle = 0;
volatile uint16 T_ON = 0;
volatile uint16 T_OFF = 0;
volatile uint16 T_TOTAL = 0;
volatile uint8 PulseState = 0;


void PulseMeasure(void)
{

	PulseState++;

	switch (PulseState)
	{
	case 1:
		Icu_SelectEdge(FALLING);
		Icu_Clear_TimerValue();

		break;

	case 2:
		T_ON = Icu_Get_Value();
		Icu_SelectEdge(RISING);
		Icu_Clear_TimerValue();
	
		break;

	case 3:
		T_OFF = Icu_Get_Value();
		Icu_SelectEdge(FALLING);
		Icu_Clear_TimerValue();

		break;
		
	case 4:
		Icu_SelectEdge(RISING);
		Icu_Clear_TimerValue();
		PulseState = 0;

		break;

	default:
		/* Do Nothing */
		break;
	}
}

int main(void)
{
	char arr[10];

	LCD_Init();
	LCD_Clear_Display();

	Icu_init(F_CPU_8);
	Icu_SelectEdge(RISING);
	Icu_Clear_TimerValue();
	Icu_Set_Cbk(PulseMeasure);

	

	while(1)
	{
		DutyCycle = ( ( (float)(T_ON)/( (float)T_ON + (float)T_OFF) ) * 100.00);


		LCD_Send_String_Row_Column(0,0,"DutyCycle is :");
		LCD_IntToString((int)DutyCycle, arr);
		LCD_GoTO_Row_Colunmn(1,0);
		LCD_Send_String(arr);
		LCD_Send_String(" %");

	}

	return 0;
}
