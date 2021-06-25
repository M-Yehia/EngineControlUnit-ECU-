 /*------------------------------------------------------------------------------
 -
 - Module: ICU
 -
 - File Name: icu.c
 -
 - Description: Source file for the AVR ICU driver
 -
 -
 -------------------------------------------------------------------------------*/

#include "ICU.h"

/*-------------------------------------------------------------------------------
                             Global Variables
 --------------------------------------------------------------------------------*/

/* Global variables to hold the address of the call back function in the application */
static void (*g_ICU1_callBackPtr)(void) = NULL_PTR;

static void (*g_ICU3_callBackPtr)(void) = NULL_PTR;

/*-------------------------------------------------------------------------------
                         Interrupt Service Routines
 --------------------------------------------------------------------------------*/

ISR(TIMER1_CAPT_vect)
{
	if(g_ICU1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_ICU1_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER3_CAPT_vect)
{
	if(g_ICU3_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_ICU3_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/*-------------------------------------------------------------------------------
                        Functions Definitions
 --------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------
                                ICU TIMER 1
--------------------------------------------------------------------------------*/
/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required clock.
 * 	2. Set the required edge detection.
 * 	3. Enable the Input Capture Interrupt.
 * 	4. Initialize Timer1 Registers
 */
void ICU1_init(const ICU_ConfigType * Config_Ptr)
{
	/* Configure ICP3/PE3 as i/p pin */
	CLEAR_BIT(DDRE,PD4);
	//SET_BIT(PORTE,PD4);

	/* Timer1 always operates in Normal Mode */
	TCCR1C = (1<<FOC1A) | (1<<FOC1B) | (1<<FOC1C);

	/*
	 * insert the required clock value in the first three bits (CS30, CS31 and CS32)
	 * of TCCR1B Register
	 */
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->clock);
	/*
     * insert the required edge type in ICES3 bit in TCCR3B Register
	 */
	TCCR1B = (TCCR1B & 0xBF) | ((Config_Ptr->edge)<<6);

	/* Initial Value for Timer1 */
	TCNT1 = 0;

	/* Initial Value for the input capture register */
	ICR1 = 0;

	/* Enable the Input Capture interrupt to generate an interrupt when edge is detected on ICP1/PD6 pin */
	TIMSK |= (1<<TICIE1);
}

/*
 * Description: Function to set the Call Back function address.
 */
void ICU1_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_ICU1_callBackPtr = a_ptr;
}

/*
 * Description: Function to set the required edge detection.
 */
void ICU1_setEdgeDetectionType( Icu_EdgeType a_edgeType)
{
	/*
	 * insert the required edge type in ICES1 bit in TCCR1B Register
	 */
	TCCR1B = (TCCR1B & 0xBF) | (a_edgeType<<6);
}

/*
 * Description: Function to get the Timer1 Value when the input is captured
 *              The value stored at Input Capture Register ICR1
 */
uint16 ICU1_getInputCaptureValue(void)
{
	return ICR1;
}

/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */
void ICU1_clearTimerValue(void)
{
	TCNT1 = 0;
}

/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
void ICU1_DeInit(void)
{
	/* Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1C = 0;
	TCNT1 = 0;
	ICR1 = 0;

	/* Disable the Input Capture interrupt */
	ETIMSK &= ~(1<<TICIE1);
}


/*-------------------------------------------------------------------------------
                                ICU TIMER 3
--------------------------------------------------------------------------------*/
/*
* Description : Function to initialize the ICU driver
* 	1. Set the required clock.
* 	2. Set the required edge detection.
* 	3. Enable the Input Capture Interrupt.
* 	4. Initialize Timer1 Registers
*/
void ICU3_init(const ICU_ConfigType * Config_Ptr)
{
	/* Configure ICP3/PE3 as i/p pin */
	CLEAR_BIT(DDRE,PE7);
	//SET_BIT(PORTE,PE7);

	/* Timer1 always operates in Normal Mode */
	TCCR3C = (1<<FOC3A) | (1<<FOC3B) | (1<<FOC3C);

	/*
	 * insert the required clock value in the first three bits (CS30, CS31 and CS32)
	 * of TCCR1B Register
	 */
	TCCR3B = (TCCR3B & 0xF8) | (Config_Ptr->clock);
	/*
    * insert the required edge type in ICES3 bit in TCCR3B Register
	 */
	TCCR3B = (TCCR3B & 0xBF) | ((Config_Ptr->edge)<<6);

	/* Initial Value for Timer1 */
	TCNT3 = 0;

	/* Initial Value for the input capture register */
	ICR3 = 0;

	/* Enable the Input Capture interrupt to generate an interrupt when edge is detected on ICP1/PD6 pin */
	ETIMSK |= (1<<TICIE3);
}

/*
* Description: Function to set the Call Back function address.
*/
void ICU3_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_ICU3_callBackPtr = a_ptr;
}

/*
* Description: Function to set the required edge detection.
*/
void ICU3_setEdgeDetectionType(Icu_EdgeType a_edgeType)
{
	/*
	 * insert the required edge type in ICES1 bit in TCCR1B Register
	 */
	TCCR3B = (TCCR3B & 0xBF) | (a_edgeType<<6);
}

/*
* Description: Function to get the Timer1 Value when the input is captured
*              The value stored at Input Capture Register ICR1
*/
uint16 ICU3_getInputCaptureValue(void)
{
	return ICR3;
}

/*
* Description: Function to clear the Timer1 Value to start count from ZERO
*/
void ICU3_clearTimerValue(void)
{
	TCNT3 = 0;
}

/*
* Description: Function to disable the Timer1 to stop the ICU Driver
*/
void ICU3_DeInit(void)
{
	/* Clear All Timer1 Registers */
	TCCR3A = 0;
	TCCR3B = 0;
	TCCR3C = 0;
	TCNT3 = 0;
	ICR3 = 0;

	/* Disable the Input Capture interrupt */
	ETIMSK &= ~(1<<TICIE3);
}
