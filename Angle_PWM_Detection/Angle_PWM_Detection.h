/*-------------------------------------------------------------------------
 - File Name: Angle_Detection.h
 -
 - Description: Header file for ECU signal calibration
 -
 --------------------------------------------------------------------------*/

#ifndef ANGLE_PWM_DETECTION_H_
#define ANGLE_PWM_DETECTION_H_

#include "TIMERS.h"
#include "UART.h"
#include "INT.h"
#include "ICU.h"

void PulseMeasure(void);
void angleDetection_INT0(void);
void dutyCycle_Calc(void);
void revCounter_TIMER2 (void);


#endif /*ANGLE_PWM_DETECTION_H_ */
