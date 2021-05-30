/*-------------------------------------------------------------------------
 - File Name: micor_config.h
 -
 - Description: Main Libraries used in every project using AVR MC
 -
 - Author: Mohamed Yehia
 -------------------------------------------------------------------------*/
#ifndef MICRO_CONFIG_H_
#define MICRO_CONFIG_H_

#ifndef F_CPU
#define F_CPU 8000000UL /*8MHz Clock frequency*/
#endif

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/iom128.h>

#endif /* MICRO_CONFIG_H_ */
