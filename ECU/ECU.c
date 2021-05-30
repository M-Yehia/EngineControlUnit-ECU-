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
uint8 ignition_duty_cycle;
uint8 injection_cylinder1_4_angle_on;
uint8 injection_cylinder1_4_angle_off;
uint8 injection_cylinder2_3_angle_on;
uint8 injection_cylinder2_3_angle_off;
uint8 ignition_cylinder1_4_angle_on;
uint8 ignition_cylinder1_4_angle_off;
uint8 ignition_cylinder2_3_angle_on;
uint8 ignition_cylinder2_3_angle_off;

uint8  g_edgeCount = 0;
uint16 g_timeHigh = 0;
uint16 g_timePeriod = 0;
uint16 g_timePeriodPlusHigh = 0;


/*-------------------------------------------------------------------------
                             Main Function
 --------------------------------------------------------------------------*/

uint8 main (void)
{
	sei();

	ICU_ConfigType icu_config = {.clock=ICU_F_CPU_8, .edge=FALLING};
	ICU_init(&icu_config);
	TIMER_ConfigType timer2_config = { .clock=EXTERNAL_RISING_EDGE, .mode=COMP, .OCRValue = 60};
	TIMER2_init(&timer2_config);
	TIMER2_callBack(revCounter_TIMER2);

	TIMER_ConfigType timer1_config = { .clock=EXTERNAL_RISING_EDGE, .mode=PWM, .ICR1Value=60 ,.OCRValue =injection_duty_cycle, .OC=NON_INVERTING,
	                                   .OCR1BValue=ignition_duty_cycle , .OC1B=NON_INVERTING};
	TIMER1_init(&timer1_config);

	DDRA = 0xFF;

	startEngine();
	while(1)
	{

		/**/
		if (rev==0)
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
				SET_BIT(PORTA,PA3);
			}
			else if(TCNT2 == injection_cylinder1_4_angle_off)
			{
				CLEAR_BIT(PORTA,PA3);
			}
			/**/
			else if(TCNT2 == ignition_cylinder2_3_angle_on)
			{
				SET_BIT(PORTA,PA6);
			}
			else if(TCNT2 == ignition_cylinder2_3_angle_off)
			{
				CLEAR_BIT(PORTA,PA6);
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

		/**/
		if (rev==1)
		{
			/**/
			if(TCNT2 == ignition_cylinder1_4_angle_on)
			{
				SET_BIT(PORTA,PA7);
			}
			else if(TCNT2 == ignition_cylinder1_4_angle_off)
			{
				CLEAR_BIT(PORTA,PA7);
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
				SET_BIT(PORTA,PA2);
			}
			else if(TCNT2 == injection_cylinder2_3_angle_off)
			{
				CLEAR_BIT(PORTA,PA2);
			}
		}
	}
}

/*-------------------------------------------------------------------------
                              Functions
 --------------------------------------------------------------------------*/

void startEngine (void)
{
		uint32 period = 0;

		/* Set the Call back function pointer in the ICU driver */
		ICU_setCallBack(APP_edgeProcessing);

		SET_BIT(PORTA,PA0);
		_delay_us(30);
		CLEAR_BIT(PORTA,PA0);

		while( >= period >= )
		{
			if(g_edgeCount == 4)
			{
				ICU_DeInit(); /* Disable ICU Driver */
				g_edgeCount = 0;
				/* calculate the period */
				period = ((g_timePeriodPlusHigh - g_timeHigh) / 1000);
			}
		}
}

void APP_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		ICU_clearTimerValue();
		/* Detect falling edge */
		ICU_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = ICU_getInputCaptureValue();
		/* Detect rising edge */
		ICU_setEdgeDetectionType(RISING);
	}
	else if(g_edgeCount == 3)
	{
		/* Store the Period time value */
		g_timePeriod = ICU_getInputCaptureValue();
		/* Detect falling edge */
		ICU_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 4)
	{
		/* Store the Period time value + High time value */
		g_timePeriodPlusHigh = ICU_getInputCaptureValue();
		/* Clear the timer counter register to start measurements again */
		ICU_clearTimerValue();
		/* Detect rising edge */
		ICU_setEdgeDetectionType(RISING);
	}
}

void revCounter_TIMER2 (void)
{
	rev++;
	if (rev == 2)
		rev = 0;
}

