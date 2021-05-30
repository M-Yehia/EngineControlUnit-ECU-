/*
 * Icu.h
 *
 *  Created on: 5 May 2021
 *      Author: OMAR
 */


#ifndef ICU_H_
#define ICU_H_

#include "std_types.h"

typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Icu_Clock_t;

typedef enum
{
	FALLING,
	RISING

}Icu_Edge_t;

void 	Icu_init(Icu_Clock_t Icu_Clock);
void 	Icu_SelectEdge(Icu_Edge_t Icu_Edge);
uint16	Icu_Get_Value(void);
void	Icu_Clear_TimerValue(void);
void 	Icu_Set_Cbk(void (*Cbk)(void));


#endif /* ICU_H_ */
