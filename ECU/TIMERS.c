/*-------------------------------------------------------------------------
 - File Name: TIMERS.c
 -
 - Module: Timer0,Timer2 (8-bit) ------ Timer1,Timer3 (16-bit)
 -
 - Description: Source file for the Timers driver for AtMega128
 -
 - Author: Mohamed Yehia
 -------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                            NOTES About Timer Driver
 -------------------------------------------------------------------------------*/
/*
 - Notes:		- Timer0/Timer2 Maximum time in 1 overflow is 256ms
 - 				  as 1 clock = 1ms
 - 				  ( Maximum => Using small clock = 1Mhz , and large prescaler = 1024 )
 -
 -				- TIMER1/Timer3 Maximum time in 1 overflow is 65535ms ( 65.5 Sec )
 -				  as 1 clock = 1ms
 -				  ( Maximum => Using small clock = 1Mhz , and large prescaler = 1024 )
 -
 - Pins:		- PB3/OC0  -> Square Wave and PWM Mode in Timer0
 - 				- PD7/OC2  -> Square Wave and PWM Mode in Timer2
 - 				- PD5/OC1A -> Square Wave and PWM Mode in TIMER1A
 - 				- PD4/OC1B -> Square Wave  Mode in TIMER1B
 -
 - Clock And Prescaler :
 - 				Time = Prescaler / F_CPU * Ticks
 - 				For clock=1Mhz and prescale F_CPU/1024 every count will take 1ms
 - 				Timer Clock => 1,000,000 / 1024 = 1Khz
 - 				clock timer period = 1 / 1Khz = 1ms
 - 				so put initial timer counter = 0  0 --> 255 (256ms per overflow)
 - 				so we need timer to overflow 4 times to get a 1 second
 -
 - Example for Config Struct:
 - 				// F_CPU = 8Mhz		TIMER1 CTC Mode 	1 Sec.
 - 				TIMER_ConfigType TIMER1_Config =
 - 					{.clock = F_CPU_1024, .mode = CTC, .OCRValue = 8000 };
 - 				TIMER1_Init(&TIMER1_Config);
 -------------------------------------------------------------------------------*/

#include "TIMERS.h"

/*-------------------------------------------------------------------------
                            Global Variables
 -------------------------------------------------------------------------*/

static void (*g_TIMER0_callBackPtr)(void) = NULL_PTR;
static void (*g_TIMER2_callBackPtr)(void) = NULL_PTR;
static void (*g_TIMER1_callBackPtr)(void) = NULL_PTR;
static void (*g_TIMER3_callBackPtr)(void) = NULL_PTR;
static uint8 g_T0clock, g_T1clock, g_T2clock , g_T3clock;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

/*
 *  Description : Interrupt Service Routine for TIMER0 CTC Mode
 */
ISR(TIMER0_COMP_vect)
{
	if(g_TIMER0_callBackPtr != NULL_PTR)
	{
		/* Call The Call Back function in the application after the timer value = OCR0 Value*/
		(*g_TIMER0_callBackPtr)() ;
	}
}

/*
 *  Description : Interrupt Service Routine for TIMER0 OverFlow(Normal) Mode
 */
ISR(TIMER0_OVF_vect)
{
	if(g_TIMER0_callBackPtr != NULL_PTR)
	{
		/* Call The Call Back function in the application after the timer value = 1023 */
		(*g_TIMER0_callBackPtr)() ;
	}
}

/*
 *  Description : Interrupt Service Routine for TIMER2 CTC Mode
 */
ISR(TIMER2_COMP_vect)
{
	if(g_TIMER2_callBackPtr != NULL_PTR)
	{
		/* Call The Call Back function in the application after the timer value = OCR0 Value*/
		(*g_TIMER2_callBackPtr)() ;
	}
}

/*
 *  Description : Interrupt Service Routine for TIMER2 OverFlow(Normal) Mode
 */
ISR(TIMER2_OVF_vect)
{
	if(g_TIMER2_callBackPtr != NULL_PTR)
	{
		/* Call The Call Back function in the application after the timer value = 1023 */
		(*g_TIMER2_callBackPtr)() ;
	}
}

/*
 *  Description : Interrupt Service Routine for TIMER1 CTC Mode
 */
ISR(TIMER1_COMPA_vect)
{
	if(g_TIMER1_callBackPtr != NULL_PTR)
	{
		/* Call The Call Back function in the application after the timer value = OCR1A Value*/
		(*g_TIMER1_callBackPtr)() ;
	}
}
/*
 *  Description : Interrupt Service Routine for TIMER1 CTC Mode
 */
ISR(TIMER1_COMPB_vect)
{
	if(g_TIMER1_callBackPtr != NULL_PTR)
	{
		/* Call The Call Back function in the application after the timer value = OCR1B Value*/
		(*g_TIMER1_callBackPtr)() ;
	}
}
/*
 *  Description : Interrupt Service Routine for TIMER1 CTC Mode
 */
ISR(TIMER1_COMPC_vect)
{
	if(g_TIMER1_callBackPtr != NULL_PTR)
	{
		/* Call The Call Back function in the application after the timer value = OCR1C Value*/
		(*g_TIMER1_callBackPtr)() ;
	}
}


/*
 *  Description : Interrupt Service Routine for TIMER1 OverFlow(Normal) Mode
 */
ISR(TIMER1_OVF_vect)
{
	if(g_TIMER1_callBackPtr != NULL_PTR)
	{
		/* Call The Call Back function in the application after the timer value = 65,535 */
		(*g_TIMER1_callBackPtr)() ;
	}
}

ISR(TIMER3_COMPA_vect)
{
	if(g_TIMER3_callBackPtr != NULL_PTR)
	{
		/* Call The Call Back function in the application after the timer value = OCR3A Value*/
		(*g_TIMER3_callBackPtr)() ;
	}
}
ISR(TIMER3_COMPB_vect)
{
	if(g_TIMER3_callBackPtr != NULL_PTR)
	{
		/* Call The Call Back function in the application after the timer value = OCR3B Value*/
		(*g_TIMER3_callBackPtr)() ;
	}
}
ISR(TIMER3_COMPC_vect)
{
	if(g_TIMER3_callBackPtr != NULL_PTR)
	{
		/* Call The Call Back function in the application after the timer value = OCR3C Value*/
		(*g_TIMER3_callBackPtr)() ;
	}
}

/*
 *  Description : Interrupt Service Routine for TIMER3 OverFlow(Normal) Mode
 */
ISR(TIMER3_OVF_vect)
{
	if(g_TIMER3_callBackPtr != NULL_PTR)
	{
		/* Call The Call Back function in the application after the timer value = 65,535 */
		(*g_TIMER3_callBackPtr)() ;
	}
}

/*-------------------------------------------------------------------------
                             TIMER0 Functions
 -------------------------------------------------------------------------*/
         /*------------------- Description --------------------
          -  Function to initialize TIMER0
          -  1. Set Timer initial value to 0
          -  2. Configure Control Register TCCR0 depending on
                desired configurations
          -  3. Enable Interrupts depending on selected mode
          ----------------------------------------------------*/
void TIMER0_init (TIMER_ConfigType *config_ptr)
{
	g_T0clock = (config_ptr ->clock);

	/* Set Timer initial value to 0 */
	TCNT0 = 0;
/*--------------------- TCCR0 ----------------------
-  1. FOC0 = 1 in non-PWM modes so mask it with selected mode
-  2. WGM01:0(bit3,6) Waveform generation mode
-  3. COM01:0 Compare match output mode (OC)
-  3. CS02:0 Clock Select
----------------------------------------------------*/
	TCCR0 = (((config_ptr ->mode)<<FOC0)& 0x80) | (((config_ptr ->mode)<<WGM00)& 0x40)
			| (((config_ptr ->mode)<<(WGM01-1)) & 0x08)
			| ((config_ptr ->OC)<<COM00) | ((config_ptr ->clock)<<CS00);

	/* Enable Timer overflow Interrupt if NORMAL mode selected */
	if (config_ptr ->mode == NORMAL)
	{
		SET_BIT (TIMSK,TOIE0);                  /* Timer overflow Interrupt Enable */
	}

	/* Enable Compare match Interrupt if Compare mode selected */
	else if (config_ptr ->mode == COMP)
	{
		SET_BIT (TIMSK,OCIE0);                  /* Compare match Interrupt Enable */
		OCR0 = (uint8)(config_ptr ->OCRValue); /* Compare Value */
	}

	/* Square wave generator (Toggle) */
	else if ((config_ptr ->mode == COMP) & (config_ptr ->OC == TOGGLE))
	{
		SET_BIT (TIMSK,OCIE0);                  /* Compare match Interrupt Enable */
		SET_BIT (DDRB,PB4);                     /* Configure PB4/OC0 Pin as output pin */
		OCR0 = (uint8)(config_ptr ->OCRValue); /* Compare Value */
	}

	/* PWM mode specials */
	else if (config_ptr ->mode == PWM)
	{
		SET_BIT (DDRB,PB4);                     /* Configure PB4/OC0 Pin as output pin */
		OCR0 = (uint8)(config_ptr ->OCRValue); /* Duty Cycle Value */
	}
}

         /*------------------- Description --------------------
                  Function to reset timer to 0 again
          ----------------------------------------------------*/
void TIMER0_resetTimer(void)
{
	TCNT0 = 0;
}

         /*------------------- Description --------------------
              Function to stop timer (Set Clock to NO_CLOCK)
          ----------------------------------------------------*/
void TIMER0_stopTimer(void)
{
	/* Clear Clock Bits */
	TCCR0 &= 0xF8;
}

         /*------------------- Description --------------------
          Function to start timer (Set Clock to g_TIMER0_Clock)
          ----------------------------------------------------*/
void TIMER0_restartTimer(void)
{
	/* Clear Clock Bits */
	TCCR0 &= 0xF8 ;

	/* Set Clock Bits */
	TCCR0 |= (g_T0clock << CS00 ) ;
}

         /*------------------- Description --------------------
              Function to change compare value/duty cycle
          ----------------------------------------------------*/
void TIMER0_ticks(const uint8 Ticks)
{
	OCR0 = Ticks;
}

         /*------------------- Description --------------------
             Function to call back function to be use in ISR
          ----------------------------------------------------*/
void TIMER0_callBack (void (*ptr)(void))
{
	/* Save the address of the function to be Called back  in a global variable */
	g_TIMER0_callBackPtr = ptr;
}

         /*------------------- Description --------------------
             Function to call back function to be use in ISR
          ----------------------------------------------------*/
void TIMER0_deinit (void)
{
	TCCR0 = 0x00;
}
/*-------------------------------------------------------------------------
                             TIMER2 Functions
 --------------------------------------------------------------------------*/
         /*------------------- Description --------------------
          -  Function to initialize TIMER2
          -  1. Set Timer initial value to 0
          -  2. Configure Control Register TCCR2 depending on
                desired configurations
          -  3. Enable Interrupts depending on selected mode
          ----------------------------------------------------*/
void TIMER2_init (TIMER_ConfigType *config_ptr)
{
	g_T2clock = (config_ptr ->clock);
	/* Set Timer initial value to 0 */
	TCNT2 = 0;
/*--------------------- TCCR2 ----------------------
-  1. FOC2 = 1 in non-PWM modes so mask it with selected mode
-  2. WGM21:0(bit3,6) Waveform generation mode
-  3. COM21:0 Compare match output mode (OC)
-  3. CS22:0 Clock Select
----------------------------------------------------*/
	TCCR2 = (((config_ptr ->mode)<<FOC2)& 0x80) | (((config_ptr ->mode)<<WGM20)& 0x40)
			| (((config_ptr ->mode)<<(WGM21-1)) & 0x08)
			| ((config_ptr ->OC)<<COM20) | ((config_ptr ->clock)<<CS20);

	/* Enable Timer overflow Interrupt if NORMAL mode selected */
	if (config_ptr ->mode == NORMAL)
	{
		SET_BIT (TIMSK,TOIE2);                  /* Timer overflow Interrupt Enable */
	}

	/* Enable Compare match Interrupt if Compare mode selected */
	else if (config_ptr ->mode == COMP)
	{
		SET_BIT (TIMSK,OCIE2);
		SET_BIT (DDRB,PB7);                    /* Compare match Interrupt Enable */
		OCR2 = (uint8)(config_ptr ->OCRValue); /* Compare Value */
	}

	/* PWM mode specials */
	else if (config_ptr ->mode == PWM)
	{
		SET_BIT (DDRB,PB7);                     /* Configure PB7/OC2 Pin as output pin */
		OCR2 = (uint8)(config_ptr ->OCRValue); /* Duty Cycle Value */
	}
}

         /*------------------- Description --------------------
                  Function to reset timer to 0 again
          ----------------------------------------------------*/
void TIMER2_resetTimer(void)
{
	TCNT2 = 0;
}

         /*------------------- Description --------------------
              Function to stop timer (Set Clock to NO_CLOCK)
          ----------------------------------------------------*/
void TIMER2_stopTimer(void)
{
	/* Clear Clock Bits */
	TCCR2 &= 0xF8;
}

         /*------------------- Description --------------------
          Function to start timer (Set Clock to g_TIMER2_Clock)
          ----------------------------------------------------*/
void TIMER2_restartTimer(void)
{
	/* Clear Clock Bits */
	TCCR2 &= 0xF8 ;

	/* Set Clock Bits */
	TCCR2 |= (g_T2clock << CS00 ) ;
}

         /*------------------- Description --------------------
              Function to change compare value/duty cycle
          ----------------------------------------------------*/
void TIMER2_ticks(const uint8 Ticks)
{
	OCR2 = Ticks;
}

         /*------------------- Description --------------------
             Function to call back function to be use in ISR
          ----------------------------------------------------*/
void TIMER2_callBack (void (*ptr)(void))
{
	/* Save the address of the function to be Called back  in a global variable */
	g_TIMER2_callBackPtr = ptr;
}

         /*------------------- Description --------------------
             Function to call back function to be use in ISR
          ----------------------------------------------------*/
void TIMER2_deinit (void)
{
	TCCR2 = 0x00;
}

/*-------------------------------------------------------------------------
                             TIMER1 Functions
 --------------------------------------------------------------------------*/
/*
 * Description : Function to initialize the Timer driver
 * 	1. Select Timer Mode ( Normal - Compare - Square - PWM )
 * 	2. Set the required clock.
 * 	3. Set Compare Value if The Timer in Compare Mode
 * 	4. Set OC1A/OC1B Pin Mode in Square Mode
 * 	5. Enable the Timer Normal-Compare Interrupt.
 * 	6. Initialize TIMER1 Registers
 */
void TIMER1_init(TIMER_ConfigType *Config_ptr)
{
	/* set a global variable for clock to use it in
	 * restart timer function */
	g_T1clock = Config_ptr->clock ;

	/* Set TIMER1 In Normal Mode */
	if (Config_ptr->mode == NORMAL)
	{
		/* Set Timer initial value to 0 */
		TCNT1 = 0;

		/*  Enable TIMER1 Overflow Interrupt */
		TIMSK |= (1<<TOIE1);

		/* Configure the timer control register
		 * 1. Non PWM mode	=> FOC1A=1 & FOC1B=1
		 * 2. Normal Mode	=> WGM11=0 & WGM10=0 & WGM13=0 & WGM12=0
		 * 3. OC1A/B Mode 	=> COM1A1 & COM1A0 & COM1B1 & COM1B0 (Disconnected)
		 * 4. clock 		=> CS12 & CS11 & CS10
		 * TCCR1A => COM1A1 COM1A0 COM1B1 COM1B0 FOC1A FOC1B WGM11 WGM10
		 * TCCR1B => ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
		 */

		TCCR1C = (1<<FOC1A) | (1<<FOC1B) | (1<<FOC1C) ;
		TCCR1B = ((Config_ptr->clock) << CS10) ;
	}

	/* Set TIMER1 In Compare Mode with OCR1A Value */
	else if (Config_ptr->mode == COMP)
	{

		/* Channels interrupt Enable */
		TIMSK |= (1 << OCIE1A) | (1 << OCIE1B) | (1 << OCIE1C);

		/* Initial Value of The Timer TCNT*/
		TCNT1 = 0;

		/* Compare Value for Channel A */
		OCR1A = Config_ptr->OCRValue;
		/* Compare Value for Channel B */
		OCR1B = Config_ptr->OCR1BValue;
		/* Compare Value for Channel C */
		OCR1C = Config_ptr->OCR1CValue;
		/* using ICR1 as Top Value for Duty Cycle for all Channels
		 * Duty Cycle for PWM Mode = OCR1x/ICR1 */
		ICR1 = Config_ptr->ICR1Value;

		/* Configure the timer control register
		 * 1. Non PWM mode	=> FOC1A=1 & FOC1B=1
		 * 2. CTC Mode		=> WGM11=0 & WGM10=0 & WGM13=0 & WGM12=1
		 * 3. OC1A/B Mode	=> COM1A1 & COM1A0 & COM1B1 & COM1B0
		 * 4. clock 		=> CS12 & CS11 & CS10
		 * TCCR1A => COM1A1 COM1A0 COM1B1 COM1B0 COM1C1 COM1C0 WGM11 WGM10
		 * TCCR1B => ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
		 * TCCR1C => FOC1A FOC1B FOC1C
		 */

		TCCR1A = ((Config_ptr->OC) << COM1A0) | ((Config_ptr->OC1B) << COM1B0)
				| ((Config_ptr->OC1C) << COM1C0);
		TCCR1B = (1 << WGM12) | ((Config_ptr->clock) << CS10);
		TCCR1C = (1 << FOC1A) | (1 << FOC1B) | (1 << FOC1C);
	}

	/* Set TIMER1 In CTC Square Wave Mode */
	else if (Config_ptr->mode == CTC)
	{
		/* Set pin PB5/OC1A as output pin */
		SET_BIT (DDRB,PB5);
		/* Set pin PB6/OC1B as output pin */
		SET_BIT (DDRB,PB6);
		/* Set pin PB7/OC1A/OC2 as output pin */
		SET_BIT (DDRB,PB7);

		/* Channels interrupt Enable */
		TIMSK |= (1<<OCIE1A) | (1<<OCIE1B) | (1<<OCIE1C);

		/* Initial Value of The Timer TCNT*/
		TCNT1 = 0 ;

		/* Compare Value for Channel A */
		OCR1A = Config_ptr->OCRValue ;
		/* Compare Value for Channel B */
		OCR1B = Config_ptr->OCR1BValue;
		/* Compare Value for Channel C */
		OCR1C = Config_ptr->OCR1CValue;

		/* Configure the timer control register
		 * 1. Non PWM mode	=> FOC1A=1 & FOC1B=1
		 * 2. CTC Mode		=> WGM11=0 & WGM10=0 & WGM13=0 & WGM12=1
		 * 3. OC1A/B Mode	=> COM1A1 & COM1A0 & COM1B1 & COM1B0
		 * 4. clock 		=> CS12 & CS11 & CS10
		 * TCCR1A => COM1A1 COM1A0 COM1B1 COM1B0 COM1C1 COM1C0 WGM11 WGM10
		 * TCCR1B => ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
		 * TCCR1C => FOC1A FOC1B FOC1C
		 */

		TCCR1A = ((Config_ptr->OC) << COM1A0) | ((Config_ptr->OC1B) << COM1B0) | ((Config_ptr->OC1C) << COM1C0);
		TCCR1B = (1<<WGM12) | ((Config_ptr->clock) << CS10) ;
		TCCR1C = (1<<FOC1A) | (1<<FOC1B) | (1<<FOC1C);
	}

	/* Set TIMER1 In PWM Mode with Duty Cycle */
	else if (Config_ptr->mode == PWM)
	{
		/* Set pin PB5/OC1A as output pin */
		SET_BIT (DDRB,PB5);
		/* Set pin PB6/OC1B as output pin */
		SET_BIT (DDRB,PB6);
		/* Set pin PB7/OC1A/OC2 as output pin */
		SET_BIT (DDRB,PB7);

		/* Initial Value of The Timer TCNT*/
		TCNT1 = 0 ;

		TIMSK |= (1<<OCIE1A) | (1<<OCIE1B) | (1<<OCIE1C);

		/* Duty Cycle Value for Channel A */
		OCR1A = Config_ptr->OCRValue;
		/* Duty Cycle Value for Channel B */
		OCR1B = Config_ptr->OCR1BValue;
		/* Duty Cycle Value for Channel C */
		OCR1C = Config_ptr->OCR1CValue;

		/* using ICR1 as Top Value for Duty Cycle for all Channels
		 * Duty Cycle for PWM Mode = OCR1x/ICR1 */
		ICR1 = Config_ptr->ICR1Value;

		/* Configure the timer control register
		 * 1. PWM mode		=> FOC1A=0 & FOC1B=0
		 * 2. PWM Mode		=> WGM11=1 & WGM10=0 & WGM13=1 & WGM12=1
		 * 3. OC1A/B/C Mode	=> COM1A1 & COM1A0 & COM1B1 & COM1B0 & COM1C1 & COM1C0
		 * 4. clock 		=> CS12 & CS11 & CS10
		 * TCCR1A => COM1A1 COM1A0 COM1B1 COM1B0 COM1C1 COM1C0 WGM11 WGM10
		 * TCCR1B => ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
		 *
		 */
		TCCR1A = ((Config_ptr->OC) << COM1A0) | ((Config_ptr->OC1B) << COM1B0)
				|((Config_ptr->OC1C) << COM1C0) | (1<<WGM11);

		TCCR1B = (1<<WGM13) | (1<<WGM12) | ((Config_ptr->clock) << CS10) ;
	}
}

/*
 * Description: Function to clear the TIMER1 Value to start count from ZERO
 */
void TIMER1_resetTimer(void)
{
	TCNT1 = 0;
}

/*
 * Description: Function to Stop the TIMER1
 * CLEAR CS12 CS11 CS10
 * TCCR1B => ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
 */
void TIMER1_stopTimer(void)
{
	/* Clear Clock Bits */
	TCCR1B &= 0xF8 ;
}

/*
 * Description: Function to Restart the TIMER1
 * CLEAR CS12 CS11 CS10
 * TCCR1B => ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
 */
void TIMER1_restartTimer(void)
{
	/* Set Clock Bits */
	TCCR1B |= ( g_T1clock << CS10) ;
}

/*
 * Description: Function to Change Ticks (Compare Value) of Timer
 * 				using also to Change Duty Cycle in PWM Mode
 */
void TIMER1_Ticks(const uint16 Ticks1A, const uint16 Ticks1B)
{
	OCR1A = Ticks1A;
	OCR1B = Ticks1B;
}

/*
 * Description: Function to set the Call Back function address for TIMER2 .
 */
void TIMER1_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_TIMER1_callBackPtr = a_ptr;
}

         /*------------------- Description --------------------
           Function to cancel the initializations of the timer
          ----------------------------------------------------*/
void TIMER1_deinit (void)
{
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TCCR1C = 0x00;
}

/*-------------------------------------------------------------------------
                             TIMER3 Functions
 --------------------------------------------------------------------------*/
/*
 * Description : Function to initialize the Timer driver
 * 	1. Select Timer Mode ( Normal - Compare - Square - PWM )
 * 	2. Set the required clock.
 * 	3. Set Compare Value if The Timer in Compare Mode
 * 	4. Set OC1A/OC1B Pin Mode in Square Mode
 * 	5. Enable the Timer Normal-Compare Interrupt.
 * 	6. Initialize TIMER1 Registers
 */
void TIMER3_init(TIMER_ConfigType *Config_ptr)
{
	/* set a global variable for clock to use it in
	 * restart timer function */
	g_T3clock = Config_ptr->clock ;

	/* Set TIMER1 In Normal Mode */
	if (Config_ptr->mode == NORMAL)
	{
		/* Set Timer initial value to 0 */
		TCNT3 = 0;

		/*  Enable TIMER3 Overflow Interrupt */
		TIMSK |= (1<<TOIE3);

		/* Configure the timer control register
		 * 1. Non PWM mode	=> FOC3A=1 & FOC3B=1
		 * 2. Normal Mode	=> WGM31=0 & WGM30=0 & WGM33=0 & WGM32=0
		 * 3. OC3A/B Mode 	=> COM3A1 & COM3A0 & COM3B1 & COM3B0 (Disconnected)
		 * 4. clock 		=> CS32 & CS31 & CS30
		 * TCCR3A => COM3A1 COM3A0 COM3B1 COM3B0 FOC3A FOC3B WGM31 WGM30
		 * TCCR3B => ICNC3 ICES3 – WGM33 WGM32 CS32 CS31 CS30
		 */

		TCCR3C = (1<<FOC3A) | (1<<FOC3B) | (1<<FOC3C) ;
		TCCR3B = ((Config_ptr->clock) << CS30) ;
	}

	/* Set TIMER3 In Compare Mode with OCR1A Value */
	else if (Config_ptr->mode == COMP)
	{
		/* Set pin PE3/OC3A as output pin */
		SET_BIT (DDRE,PE3);
		/* Set pin PE4/OC3B/INT4 as output pin */
		SET_BIT (DDRE,PE4);
		/* Set pin PE5/OC3C/INT5 as output pin */
		SET_BIT (DDRE,PE5);

		/* Channels interrupt Enable */
		TIMSK |= (1<<OCIE3A) | (1<<OCIE3B) | (1<<OCIE3C);

		/* Initial Value of The Timer TCNT*/
		TCNT3 = 0 ;

		/* Compare Value for Channel A */
		OCR3A = Config_ptr->OCRValue ;
		/* Compare Value for Channel B */
		OCR3B = Config_ptr->OCR1BValue;
		/* Compare Value for Channel C */
		OCR3C = Config_ptr->OCR1CValue;

		/* Configure the timer control register
		 * 1. Non PWM mode	=> FOC3A=1 & FOC3B=1
		 * 2. CTC Mode		=> WGM31=0 & WGM30=0 & WGM33=0 & WGM32=1
		 * 3. OC3A/B/C Mode	=> COM3A1 & COM3A0 & COM3B1 & COM3B0 & COM3C1 & COM3C0
		 * 4. clock 		=> CS32 & CS31 & CS30
		 * TCCR3A => COM3A1 COM3A0 COM3B1 COM3B0 COM3C1 COM3C0 WGM31 WGM30
		 * TCCR3B => ICNC3 ICES3 – WGM33 WGM32 CS32 CS31 CS30
		 * TCCR3C => FOC3A FOC3B FOC3C
		 */

		TCCR3A = ((Config_ptr->OC) << COM3A0) | ((Config_ptr->OC1B) << COM3B0) | ((Config_ptr->OC1C) << COM3C0);
		TCCR3B = (1<<WGM33) | (1<<WGM32) | ((Config_ptr->clock) << CS30) ;
		TCCR3C = (1<<FOC3A) | (1<<FOC3B) | (1<<FOC3C);
	}

	/* Set TIMER3 In CTC Square Wave Mode */
	else if (Config_ptr->mode == CTC)
	{
		/* Set pin PE3/OC3A as output pin */
		SET_BIT (DDRE,PE3);
		/* Set pin PE4/OC3B/INT4 as output pin */
		SET_BIT (DDRE,PE4);
		/* Set pin PE5/OC3C/INT5 as output pin */
		SET_BIT (DDRE,PE5);

		/* Channels interrupt Enable */
		TIMSK |= (1<<OCIE3A) | (1<<OCIE3B) | (1<<OCIE3C);

		/* Initial Value of The Timer TCNT*/
		TCNT3 = 0 ;

		/* Compare Value for Channel A */
		OCR3A = Config_ptr->OCRValue ;
		/* Compare Value for Channel B */
		OCR3B = Config_ptr->OCR1BValue;
		/* Compare Value for Channel C */
		OCR3C = Config_ptr->OCR1CValue;

		/* Configure the timer control register
		 * 1. Non PWM mode	=> FOC3A=1 & FOC3B=1
		 * 2. CTC Mode		=> WGM31=0 & WGM30=0 & WGM33=0 & WGM32=1
		 * 3. OC1A/B Mode	=> COM3A1 & COM3A0 & COM3B1 & COM3B0 & COM3C1 & COM3C0
		 * 4. clock 		=> CS32 & CS31 & CS30
		 * TCCR3A => COM3A1 COM3A0 COM3B1 COM3B0 COM3C1 COM3C0 WGM31 WGM30
		 * TCCR3B => ICNC3 ICES3 – WGM33 WGM32 CS32 CS31 CS30
		 * TCCR3C => FOC3A FOC3B FOC3C
		 */

		TCCR3A = ((Config_ptr->OC) << COM3A0) | ((Config_ptr->OC1B) << COM3B0) | ((Config_ptr->OC1C) << COM3C0);
		TCCR3B = (1<<WGM32) | ((Config_ptr->clock) << CS30) ;
		TCCR3C = (1<<FOC3A) | (1<<FOC3B) | (1<<FOC3C);
	}

	/* Set TIMER1 In PWM Mode with Duty Cycle */
	else if (Config_ptr->mode == PWM)
	{
		/* Set pin PE3/OC3A as output pin */
		SET_BIT (DDRE,PE3);
		/* Set pin PE4/OC3B/INT4 as output pin */
		SET_BIT (DDRE,PE4);
		/* Set pin PE5/OC3C/INT5 as output pin */
		SET_BIT (DDRE,PE5);

		/* Channels interrupt Enable */
		TIMSK |= (1<<OCIE3A) | (1<<OCIE3B) | (1<<OCIE3C);

		/* Initial Value of The Timer TCNT*/
		TCNT3 = 0 ;

		/* Duty Cycle Value for Channel A */
		OCR3A = Config_ptr->OCRValue;
		/* Duty Cycle Value for Channel B */
		OCR3B = Config_ptr->OCR1BValue;
		/* Duty Cycle Value for Channel C */
		OCR3C = Config_ptr->OCR1CValue;

		/* using ICR1 as Top Value for Duty Cycle for all Channels
		 * Duty Cycle for PWM Mode = OCR1x/ICR1 */
		ICR3 = Config_ptr->ICR1Value;

		/* Configure the timer control register
		 * 1. PWM mode		=> FOC3A=0 & FOC3B=0
		 * 2. PWM Mode		=> WGM31=1 & WGM30=0 & WGM33=1 & WGM32=1
		 * 3. OC3A/B/C Mode	=> COM3A1 & COM3A0 & COM3B1 & COM3B0 & COM3C1 & COM3C0
		 * 4. clock 		=> CS32 & CS31 & CS30
		 * TCCR3A => COM3A1 COM3A0 COM3B1 COM3B0 COM3C1 COM3C0 WGM31 WGM30
		 * TCCR3B => ICNC3 ICES3 – WGM33 WGM32 CS32 CS31 CS30
		 *
		 */
		TCCR3A = ((Config_ptr->OC) << COM3A0) | ((Config_ptr->OC1B) << COM3B0)
				|((Config_ptr->OC1C) << COM3C0) | (1<<WGM31);

		TCCR3B = (1<<WGM33) | (1<<WGM32) | ((Config_ptr->clock) << CS30) ;
	}
}
/*
 * Description: Function to clear the TIMER1 Value to start count from ZERO
 */
void TIMER3_resetTimer(void)
{
	TCNT3 = 0;
}

/*
 * Description: Function to Stop the TIMER1
 * CLEAR CS12 CS11 CS10
 * TCCR1B => ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
 */
void TIMER3_stopTimer(void)
{
	/* Clear Clock Bits */
	TCCR3B &= 0xF8 ;
}

/*
 * Description: Function to Restart the TIMER1
 * CLEAR CS12 CS11 CS10
 * TCCR1B => ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
 */
void TIMER3_restartTimer(void)
{
	/* Clear Clock Bits */
	TCCR3B &= 0xF8 ;

	/* Set Clock Bits */
	TCCR3B |= ( g_T3clock << CS30) ;
}

/*
 * Description: Function to Change Ticks (Compare Value) of Timer
 * 				using also to Change Duty Cycle in PWM Mode
 */
void TIMER3_Ticks(const uint16 Ticks3A, const uint16 Ticks3B)
{
	OCR3A = Ticks3A;
	OCR3B = Ticks3B;
}

/*
 * Description: Function to set the Call Back function address for TIMER2 .
 */
void TIMER3_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_TIMER3_callBackPtr = a_ptr;
}

         /*------------------- Description --------------------
           Function to cancel the initializations of the timer
          ----------------------------------------------------*/
void TIMER3_deinit (void)
{
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TCCR1C = 0x00;
}
