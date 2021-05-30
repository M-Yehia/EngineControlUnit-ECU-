/*
 * Icu.c
 *
 *  Created on: 5 May 2021
 *      Author: OMAR
 */


#include "Icu.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static void (*PTR)(void) = NULL;

ISR(TIMER1_CAPT_vect)
{
	if(PTR)
	{
		PTR();
	}
}

void 	Icu_init(Icu_Clock_t Icu_Clock)
{
	TCCR1A = 0;
	TCCR1B = (1<<ICNC1)|(1<<ICES1)|(Icu_Clock);
	TCCR1C = (1<<FOC1A)|(1<<FOC1B)|(1<<FOC1C);
	TCNT1 = 0;
	OCR1A = 0;
	ICR1 = 0;
	TIMSK = (1<<TICIE1);
	SREG |= (1<<7);

	DDRD &=~(1<<PD4);
}

void 	Icu_SelectEdge(Icu_Edge_t Icu_Edge)
{
	/*Clearing ICES1 Bit*/
	TCCR1B &= (0xBF);
	TCCR1B |= (Icu_Edge<<ICES1);
}

uint16	Icu_Get_Value(void)
{
	return ICR1;
}

void	Icu_Clear_TimerValue(void)
{
	TCNT1 = 0;
}

void 	Icu_Set_Cbk(void (*Cbk)(void))
{
	PTR = Cbk;
}
