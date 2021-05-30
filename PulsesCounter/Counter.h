/*
 * Counter.h
 *
 *  Created on: 5 May 2021
 *      Author: OMAR
 */

#ifndef COUNTER_H_
#define COUNTER_H_

#include <avr/io.h>

#define RISING_EDGE
//#define FALLING_EDGE

void Counter_init(unsigned char InitialValue);
void Counter_Clear(void);
unsigned char Counter_GetValue(void);

#endif /* COUNTER_H_ */
