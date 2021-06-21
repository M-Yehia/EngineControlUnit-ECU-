/*-------------------------------------------------------------------------
 - File Name: ECU.h
 -
 - Description: Source file for the ECU project
 -
 - Author: Mohamed Yehia
 -------------------------------------------------------------------------*/

#include "ECU.h"

/*-------------------------------------------------------------------------
                           Global Variables
 --------------------------------------------------------------------------*/

/* Global Variable to indicate the crank revolution   */
uint8 rev = 0;
uint8 injection_duty_cycle;
uint8 ignition_duty_cycle = 2.2 ; //3% = 1.8
uint8 injection_cylinder1_4_angle_on;
uint8 injection_cylinder1_4_angle_off;
uint8 injection_cylinder2_3_angle_on;
uint8 injection_cylinder2_3_angle_off;
uint8 ignition_cylinder1_4_angle_on = 2;
uint8 ignition_cylinder1_4_angle_off = 4;
uint8 ignition_cylinder2_3_angle_on = 32;
uint8 ignition_cylinder2_3_angle_off = 34;

uint8  g_edgeCount = 0;
uint16 g_timeHigh = 0;
uint16 g_timePeriod = 0;
uint16 g_timePeriodPlusHigh = 0;
float32 g_period =0;


/*-------------------------------------------------------------------------
                             Main Function
 --------------------------------------------------------------------------*/

uint8 main (void)
{
	sei();

	ICU_ConfigType icu3_config = {.clock=ICU_F_CPU_8, .edge=FALLING};
	ICU3_init(&icu3_config);
	ICU3_setCallBack(APP_edgeProcessing);

	TIMER_ConfigType timer2_config = { .clock=EXTERNAL_RISING_EDGE, .mode=COMP, .OCRValue = 60};
	TIMER2_init(&timer2_config);
	TIMER2_callBack(revCounter_TIMER2);

	/*--------------------- Description ----------------------
	 - Channel A -> Injection
	 - Channel B -> Ignition
	 ---------------------------------------------------------*/
	TIMER_ConfigType timer1_config = { .clock=EXTERNAL_RISING_EDGE, .mode=PWM, .ICR1Value=60 ,.OCRValue =injection_duty_cycle, .OC=NON_INVERTING,
	                                   .OCR1BValue=ignition_duty_cycle , .OC1B=NON_INVERTING};
	TIMER1_init(&timer1_config);
	TIMER1_stopTimer();

	DDRA = 0xFF;

	//startEngine();


	//ICU3_DeInit();
	while(1)
	{

		/*------------------PORTA Pins Description-----------------
		                    Firing Order (1,3,4,2)
		 - PA0 -> Injection 1
		 - PA1 -> Injection 3
		 - PA2 -> Injection 4
		 - PA3 -> Injection 2
		 - PA4 -> Ignition Cylinder 1 & 4
		 - PA5 -> Ignition Cylinder 2 & 3
		 ----------------------------------------------------------- */
		if (rev==0)
		{
			/**/
			if(TCNT2 == ignition_cylinder1_4_angle_on)
			{
				TIMER1_restartTimer();
				SET_BIT(PORTA,PA4);
			}
			else if(TCNT2 == ignition_cylinder1_4_angle_off)
			{
				CLEAR_BIT(PORTA,PA4);
			}
			/**/
			else if(TCNT2 == injection_cylinder1_4_angle_on)
			{
				SET_BIT(PORTA,PA2);
			}
			else if(TCNT2 == injection_cylinder1_4_angle_off)
			{
				CLEAR_BIT(PORTA,PA2);
			}
			/**/
			else if(TCNT2 == ignition_cylinder2_3_angle_on)
			{
				SET_BIT(PORTA,PA5);
			}
			else if(TCNT2 == ignition_cylinder2_3_angle_off)
			{
				CLEAR_BIT(PORTA,PA5);
			}
			/**/
			else if(TCNT2 == injection_cylinder2_3_angle_on)
			{
				SET_BIT(PORTA,PA3);
			}
			else if(TCNT2 == injection_cylinder2_3_angle_off)
			{
				CLEAR_BIT(PORTA,PA3);
			}
		}

		/**/
		if (rev==1)
		{
			/**/
			if(TCNT2 == ignition_cylinder1_4_angle_on)
			{
				SET_BIT(PORTA,PA4);
			}
			else if(TCNT2 == ignition_cylinder1_4_angle_off)
			{
				CLEAR_BIT(PORTA,PA4);
			}
			/**/
			else if(TCNT2 == injection_cylinder1_4_angle_on)
			{
				SET_BIT(PORTA,PA0);
			}
			else if(TCNT2 == injection_cylinder1_4_angle_off)
			{
				CLEAR_BIT(PORTA,PA0);
			}
			/**/
			else if(TCNT2 == ignition_cylinder2_3_angle_on)
			{
				SET_BIT(PORTA,PA5);
			}
			else if(TCNT2 == ignition_cylinder2_3_angle_off)
			{
				CLEAR_BIT(PORTA,PA5);
			}
			/**/
			else if(TCNT2 == injection_cylinder2_3_angle_on)
			{
				SET_BIT(PORTA,PA1);
			}
			else if(TCNT2 == injection_cylinder2_3_angle_off)
			{
				CLEAR_BIT(PORTA,PA1);
			}
		}
	}
}

/*-------------------------------------------------------------------------
                              Functions
 --------------------------------------------------------------------------*/

void startEngine (void)
{

		/* Set the Call back function pointer in the ICU driver */


		SET_BIT(PORTA,PA0);
		_delay_ms(500);
		CLEAR_BIT(PORTA,PA0);

		while(  g_period < 50.00 )
		{

				SET_BIT(PORTA,PA1);
				g_edgeCount = 0;
				/* calculate the period */
				g_period = (((float32) (g_timeHigh)) / ((float32)(g_timePeriod)+(float32)(g_timeHigh))) * 100.00;

		}
}

void APP_edgeProcessing(void)
{
	g_edgeCount++;
	switch (g_edgeCount)
	{
	case 1:
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge */
		ICU3_clearTimerValue();
		/* Detect falling edge */
		ICU3_setEdgeDetectionType(FALLING);
		break;

	case 2:
		/* Store the High time value */
		g_timeHigh = ICU3_getInputCaptureValue();
		/* Detect rising edge */
		ICU3_setEdgeDetectionType(RISING);
		ICU3_clearTimerValue();

		break;

	case 3:
		/* Store the Period time value */
		g_timePeriod = ICU3_getInputCaptureValue();
		/* Detect falling edge */
		ICU3_setEdgeDetectionType(FALLING);
		ICU3_clearTimerValue();
	    break;

	case 4:
		/* Store the Period time value + High time value */
		g_timePeriodPlusHigh = ICU3_getInputCaptureValue();
		/* Clear the timer counter register to start measurements again */
		/* Detect rising edge */
		ICU3_setEdgeDetectionType(RISING);
		ICU3_clearTimerValue();
		g_edgeCount = 0;
		/* calculate the period */
		break;

	default:
		break;
	}
}

void revCounter_TIMER2 (void)
{
	rev++;
	if (rev == 2)
		rev = 0;
}

