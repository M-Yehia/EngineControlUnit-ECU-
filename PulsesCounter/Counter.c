/*
 * Counter.c
 *
 *  Created on: 5 May 2021
 *      Author: OMAR
 */

#include "Counter.h"

void Counter_init(unsigned char InitialValue)
{
#ifdef RISING_EDGE
	TCCR2 = (1<<FOC2)|(1<<CS02)|(1<<CS01)|(1<<CS00);
#endif

#ifdef FALLING_EDGE
	TCCR2 = (1<<FOC0)|(1<<CS02)|(1<<CS01);
#endif

	TCNT2 = InitialValue;
}

void Counter_Clear(void)
{
	TCNT2 = 0;
}
unsigned char Counter_GetValue(void)
{
	return TCNT2;
}
