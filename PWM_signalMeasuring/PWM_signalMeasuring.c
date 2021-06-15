/*
 * Main.c
 *
 *  Created on: 5 May 2021
 *      Author: OMAR
 */

#include "PWM_signalMeasuring.h"


uint16 T_ON = 0;
uint16 T_OFF = 0;
uint16 T_TOTAL = 0;
uint8 PulseState = 0;
uint8  DutyCycle = 0 ;

void PulseMeasure(void)
{

	PulseState++;

	switch (PulseState)
	{
	case 1:
		ICU_setEdgeDetectionType(FALLING);
		ICU_clearTimerValue();

		break;

	case 2:
		T_ON = ICU_getInputCaptureValue();
		ICU_setEdgeDetectionType(RISING);
		ICU_clearTimerValue();

		break;

	case 3:
		T_OFF = ICU_getInputCaptureValue();
		ICU_setEdgeDetectionType(FALLING);
		ICU_clearTimerValue();

		break;

	case 4:
		ICU_setEdgeDetectionType(RISING);
		ICU_clearTimerValue();
		PulseState = 0;

		break;

	default:
		/* Do Nothing */
		break;
	}
}

int main(void)
{
	uint8 arr[10];

	ICU_ConfigType ICU_Config = { .clock=ICU_F_CPU_8,.edge=RISING };
	ICU_init(&ICU_Config);
	ICU_clearTimerValue();
	ICU_setCallBack(PulseMeasure);

	UART_ConfigType UART_Config ={.parity_mode=EVEN_PARITY,.stop_bit=_1_bit,.baud_rate=BR9600,
			.data_size=_8_bit};
	UART0_init(&UART_Config);

	DutyCycle = ( ( (float)(T_ON)/( (float)T_ON + (float)T_OFF) ) * 100.00);

	sprintf(arr, "%d", (uint8)DutyCycle);
	UART0_sendString("Duty Cycle is : ");
	UART0_sendString(arr);
	UART0_sendString(" %");
	UART0_sendByte('\r');
	while(1)
	{

	}

}
