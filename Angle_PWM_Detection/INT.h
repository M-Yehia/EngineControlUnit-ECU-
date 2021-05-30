/*
 * INT.h
 *
 *  Created on: May 28, 2021
 *      Author: Dodo
 */

#ifndef INT_H_
#define INT_H_

#include "macros.h"
#include "typedef.h"
#include "micro_config.h"

void INT0_init(void);
void INT0_setCallBack(void (*ptr)(void));

#endif /* INT_H_ */
