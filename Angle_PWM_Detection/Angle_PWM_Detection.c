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


void PulseMeasure(void)
{

	PulseState++;

	switch (PulseState)
	{
	case 1:
		ICU_setEdgeDetectionType(RISING);
		ICU_clearTimerValue();

		break;

	case 2:
		T_ON = ICU_getInputCaptureValue();
		ICU_setEdgeDetectionType(FALLING);
		ICU_clearTimerValue();

		break;

	case 3:
		T_OFF = ICU_getInputCaptureValue();
		ICU_setEdgeDetectionType(RISING);
		ICU_clearTimerValue();


		break;

	case 4:
		ICU_setEdgeDetectionType(FALLING);
		ICU_clearTimerValue();
		PulseState = 0;

		break;

	default:
		/* Do Nothing */
		break;
	}
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


	ICU_ConfigType ICU_Config = { .clock = ICU_F_CPU_8, .edge = FALLING };
	ICU_init(&ICU_Config);
	ICU_clearTimerValue();
	ICU_setCallBack(PulseMeasure);


	TIMER_ConfigType TIMER2_Config ={.clock=EXTERNAL_RISING_EDGE, .mode=COMP, .OCRValue=60 };
	TIMER2_init(&TIMER2_Config);
	TIMER2_callBack(revCounter_TIMER2);


	UART_ConfigType UART_Config ={.parity_mode=EVEN_PARITY,.stop_bit=_1_bit,.baud_rate=BR57600,
			.data_size=_8_bit};
	UART0_init(&UART_Config);
	UART0_sendString(" Graduation Project --> Signal from ECU ");


	INT0_init();
	INT0_setCallBack(angleDetection_INT0);





	while(1)
	{

	}
}


