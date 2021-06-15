/*
 * Angle_Detection.c
 *
 *  Created on: May 28, 2021
 *      Author: Dodo
 */
#include "Angle_PWM_Detection.h"

volatile uint8  rev=0;
volatile uint8  ticks=0 ;
volatile uint16 T_ON = 0;
volatile uint16 T_OFF = 0;
volatile uint16 T_TOTAL = 0;
volatile uint8  PulseState = 0;
volatile uint8  DutyCycle = 0 ;
volatile uint8  g_edgeCount = 0;
volatile uint8  g_timeHigh = 0;
volatile uint8  g_timePeriodPlusHigh = 0;
volatile uint8  g_timePeriod = 0;
volatile float32  g_period = 0;

void PulseMeasure(void)
{

	PulseState++;

	switch (PulseState)
	{
	case 1:
		ICU1_setEdgeDetectionType(FALLING);
		ICU1_clearTimerValue();

		break;

	case 2:
		T_ON = ICU1_getInputCaptureValue();
		ICU1_setEdgeDetectionType(RISING);
		ICU1_clearTimerValue();

		break;

	case 3:
		T_OFF = ICU1_getInputCaptureValue();
		ICU1_setEdgeDetectionType(FALLING);
		ICU1_clearTimerValue();


		break;

	case 4:
		ICU1_setEdgeDetectionType(RISING);
		ICU1_clearTimerValue();
		PulseState = 0;

		break;

	default:
		/* Do Nothing */
		break;
	}
}

void periodMeasure (void)
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
		break;

	case 3:
		/* Store the Period time value */
		g_timePeriod = ICU3_getInputCaptureValue();
		/* Detect falling edge */
		ICU3_setEdgeDetectionType(FALLING);
	    break;

	case 4:
		/* Store the Period time value + High time value */
		g_timePeriodPlusHigh = ICU3_getInputCaptureValue();
		/* Clear the timer counter register to start measurements again */
		ICU3_clearTimerValue();
		/* Detect rising edge */
		ICU3_setEdgeDetectionType(RISING);
		g_edgeCount = 0;
		/* calculate the period */
		break;

	default:
		break;
	}
}

void periodMeasure_Calc (void)
{
	ticks = TCNT2;
	uint8 arr[4];
	uint8 arr1[4];
	(g_period) = (g_timePeriodPlusHigh-g_timeHigh);

	sprintf(arr, "%d", (uint8)g_period);
	UART0_sendString("Period of each crank tooth : ");
	UART0_sendString(arr);
	UART0_sendString(" ms");
	UART0_sendByte('\r');
	sprintf(arr1, "%d", (uint8)ticks);
	UART0_sendString("Tick No. : ");
	UART0_sendString(arr1);
	UART0_sendByte('\r');
}
void angleDetection_INT0(void)
{
	uint8 arr1[3];
	uint8 arr2[3];
	ticks = TCNT2;

	UART0_sendByte('\r');
	dutyCycle_Calc();


	UART0_sendString("Number of ticks = ");
	sprintf(arr1, "%d", ticks);
	UART0_sendString(arr1);
	UART0_sendByte('\r');

	UART0_sendString("Revolution number = ");
	sprintf(arr2, "%d", rev);
	UART0_sendString(arr2);
	UART0_sendByte('\r');


}

void dutyCycle_Calc(void)
{
	uint8 arr[4];

	DutyCycle = (((float) (T_ON) / ((float) T_ON + (float) T_OFF)) * 100.00);
	sprintf(arr, "%d", (uint8)DutyCycle);
	UART0_sendString("Duty Cycle is : ");
	UART0_sendString(arr);
	UART0_sendString(" %");
	UART0_sendByte('\r');
}

void revCounter_TIMER2 (void)
{
	rev++;
	if (rev == 3)
		rev = 1;
}



int main(void)
{
	sei();


	ICU_ConfigType ICU3_Config = { .clock = ICU_F_CPU_CLOCK, .edge = RISING };
	ICU3_init(&ICU3_Config);
	ICU3_clearTimerValue();
	ICU3_setCallBack(periodMeasure);

	ICU_ConfigType ICU1_Config = { .clock = ICU_F_CPU_CLOCK, .edge = RISING };
	ICU1_init(&ICU1_Config);
	ICU1_clearTimerValue();
	ICU1_setCallBack(PulseMeasure);


	TIMER_ConfigType TIMER2_Config ={.clock=EXTERNAL_RISING_EDGE, .mode=NORMAL };
	TIMER2_init(&TIMER2_Config);
	TIMER2_callBack(revCounter_TIMER2);


	UART_ConfigType UART_Config ={.parity_mode=EVEN_PARITY,.stop_bit=_1_bit,.baud_rate=BR9600,
			.data_size=_8_bit};
	UART0_init(&UART_Config);
	UART0_sendString(" Graduation Project --> Signal from ECU ");


	INT0_init();
	INT0_setCallBack(angleDetection_INT0);


	while(1)
	{

		 dutyCycle_Calc();
		 periodMeasure_Calc();

	}
}


