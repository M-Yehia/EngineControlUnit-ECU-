/*-------------------------------------------------------------------------
 - File Name: ECU.h
 -
 - Description: Header file for the ECU project
 -
 - Author: Mohamed Yehia
 -------------------------------------------------------------------------*/

#ifndef ECU_H_
#define ECU_H_

#include "TIMERS.h"
#include "UART.h"
#include "ICU.h"

/*-------------------------------------------------------------------------
                           Functions Prototypes
 --------------------------------------------------------------------------*/

void startEngine (void);

void APP_edgeProcessing(void);

void revCounter_TIMER2 (void);


#endif /* ECU_H_ */
