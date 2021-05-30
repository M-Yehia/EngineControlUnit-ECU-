/*-------------------------------------------------------------------------
 - File Name: TIMERS.h
 -
 - Module: Timer0,Timer2 (8-bit) ------ Timer1,Timer3 (16-bit)
 -
 - Description: Header file for the Timers driver for AtMega128
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

#ifndef TIMERS_H_
#define TIMERS_H_

#include "typedef.h"
#include "macros.h"
#include "micro_config.h"

/*-------------------------------------------------------------------------
                            Types Declaration
 -------------------------------------------------------------------------*/

typedef enum{
	NORMAL, CTC, COMP, PWM
}TIMER_Mode;

typedef enum{
	DISCONNECTED = 0, TOGGLE = 1, CLEAR = 2, SET = 3 , NON_INVERTING = 2 , INVERTING = 3
}OC_Pin_Mode;

typedef enum{
	NO_CLOCK, F_CPU_CLOCK, F_CPU_8, F_CPU_T2_64,F_CPU_T2_256, F_CPU_T2_1024,EXTERNAL_FALLING_EDGE,EXTERNAL_RISING_EDGE

	/* additional Clock For TIMER2 */
	,F_CPU_T0_32=3, F_CPU_T0_64,F_CPU_T0_128 ,F_CPU_T0_256, F_CPU_T0_1024
}TIMER_Clock;


typedef struct{
	/* For TIMER0:
	 - NO_CLOCK, F_CPU_CLOCK, F_CPU_8, F_CPU_32 , F_CPU_64, F_CPU_128 , F_CPU_256, F_CPU_1024
	 - For TIMER2:
	 - F_CPU_T2_64, F_CPU_T2_256, F_CPU_T2_1024, EXTERNAL_FALLING_EDGE,EXTERNAL_RISING_EDGE
	 */
	TIMER_Clock clock ;

	/* NORMAL, COMP, CTC, PWM */
	TIMER_Mode mode ;

	/* values 0:255 ***** But in OCR1A 0:65535
	 * using also for set Duty Cycle in PWM Mode
	 * using as OCR0 , OCR1A , OCR2 Value */
	uint16 OCRValue ;

	/* CTC Mode => TOGGLE, CLEAR, SET
	 * PWM Mode => NON_INVERTING, INVERTING
	 * using as OC0 , OC1A , OC2 */
	OC_Pin_Mode OC ;

	/* values 0:65535
	 * using for OCR1B value in TIMER1
	 * and for ICR1 value as TOP value in TIMER1 PWM Duty Cycle */
	uint16 OCR1BValue ;

	/* CTC Mode => TOGGLE, CLEAR, SET
	 * PWM Mode => NON_INVERTING, INVERTING */
	OC_Pin_Mode OC1B ;

	/* values 0:65535
	 * using for OCR1C value in TIMER1 */
	uint16 OCR1CValue ;

	/* CTC Mode => TOGGLE, CLEAR, SET
	 * PWM Mode => NON_INVERTING, INVERTING */
	OC_Pin_Mode OC1C ;

	/* Top Value for Duty Cycle in TIMER1/TIMER3 */
	uint16 ICR1Value;

}TIMER_ConfigType;


/*-------------------------------------------------------------------------
                       Timer0 Functions Prototypes
 -------------------------------------------------------------------------*/

void TIMER0_init(TIMER_ConfigType *Config_ptr);

void TIMER0_resetTimer(void);

void TIMER0_stopTimer(void);

void TIMER0_restartTimer(void);

void TIMER0_ticks(const uint8 Ticks);

void TIMER0_callBack(void(*a_ptr)(void));

void TIMER0_deinit (void);

/*-------------------------------------------------------------------------
                       Timer2 Functions Prototypes
 -------------------------------------------------------------------------*/

/*
 * Description : Function to initialize the Timer driver
 * 	1. Select Timer Mode ( Normal - Compare - Square - PWM )
 * 	2. Set the required clock.
 * 	3. Set Compare Value if The Timer in Compare Mode
 * 	4. Set OC2 Pin Mode in Square Mode
 * 	5. Enable the Timer Normal-Compare Interrupt.
 * 	6. Initialize Timer2 Registers
 */
void TIMER2_init(TIMER_ConfigType *Config_ptr);

void TIMER2_resetTimer(void);

void TIMER2_stopTimer(void);

void TIMER2_restartTimer(void);

void TIMER_ticks(const uint8 Ticks);

void TIMER2_callBack(void(*a_ptr)(void));

void TIMER2_deinit (void);

/*-------------------------------------------------------------------------
                       Timer1 Functions Prototypes
 -------------------------------------------------------------------------*/

/*
 * Description : Function to initialize the Timer driver
 * 	1. Select Timer Mode ( Normal - Compare - Square - PWM )
 * 	2. Set the required clock.
 * 	3. Set Compare Value if The Timer in Compare Mode
 * 	4. Set OC1A/B Pin Mode in Square Mode
 * 	5. Enable the Timer Normal-Compare Interrupt.
 * 	6. Initialize TIMER1 Registers
 */
void TIMER1_init(TIMER_ConfigType *Config_ptr);

void TIMER1_resetTimer(void);

void TIMER1_stopTimer(void);

void TIMER1_restartTimer(void);

void TIMER1_Ticks(const uint16 Ticks1A, const uint16 Ticks1B);

void TIMER1_setCallBack(void(*a_ptr)(void));

void TIMER1_deinit (void);

/*-------------------------------------------------------------------------
                       Timer3 Functions Prototypes
 -------------------------------------------------------------------------*/

/*
 * Description : Function to initialize the Timer driver
 * 	1. Select Timer Mode ( Normal - Compare - Square - PWM )
 * 	2. Set the required clock.
 * 	3. Set Compare Value if The Timer in Compare Mode
 * 	4. Set OC1A/B Pin Mode in Square Mode
 * 	5. Enable the Timer Normal-Compare Interrupt.
 * 	6. Initialize TIMER1 Registers
 */
void TIMER3_init(TIMER_ConfigType *Config_ptr);

void TIMER3_resetTimer(void);

void TIMER3_stopTimer(void);

void TIMER3_restartTimer(void);

void TIMER3_Ticks(const uint16 Ticks1A, const uint16 Ticks1B);

void TIMER3_setCallBack(void(*a_ptr)(void));

void TIMER3_deinit (void);

#endif /* TIMERS_H_ */
