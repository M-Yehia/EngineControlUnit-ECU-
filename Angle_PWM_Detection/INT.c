/*
 * INT.c
 *
 *  Created on: May 28, 2021
 *      Author: Dodo
 */
#include "INT.h"

static void (*g_INT_callBackPtr)(void) = NULL_PTR;

ISR(INT0_vect)
{
	if(g_INT_callBackPtr != NULL_PTR)
		{
			/* Call The Call Back function in the application after the timer value = OCR0 Value*/
			(*g_INT_callBackPtr)() ;
		}
}

void INT0_init(void)
{
	SET_BIT(EICRA,ISC01);
	SET_BIT(EIMSK,INT0);
	SET_BIT(PORTD,PD0);

}



void INT0_setCallBack(void (*ptr)(void))
{
	g_INT_callBackPtr= ptr;
}

