/*
 * common_macros.h
 *
 *   Created on: 5 May 2021
 *      Author: OMAR
 */

#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

#define SET_BIT(Register,Pin)       (Register |=(1<<Pin))
#define CLEAR_BIT(Register,Pin)     (Register &=~(1<<Pin))
#define TOGGLE_BIT(Register,Pin)	(Register ^=(1<<Pin))
#define BIT_SET(Register,Pin)       (Register &(1<<Pin))
#define BIT_CLEARED(Register,Pin)   (!(Register &(1<<Pin)))


#endif /* COMMON_MACROS_H_ */
