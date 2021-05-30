/*-------------------------------------------------------------------------
 - File Name: macros.c
 -
 - Description: Most used common macros
 -
 - Author: Mohamed Yehia
 -------------------------------------------------------------------------*/
#ifndef MACROS_H_
#define MACROS_H_

/* NULL define */
#ifndef NULL_PTR
#define NULL_PTR ((void*)(0))
#endif

#define SET_BIT(REG,BIT_NUM)       (REG |= (1 << BIT_NUM))

#define CLEAR_BIT(REG,BIT_NUM)     (REG &= (~(1 << BIT_NUM)))

#define TOGGLE_BIT(REG,BIT_NUM)    (REG ^= (1 << BIT_NUM))

#define ROTATE_BIT_R(REG,BIT_NUM)  (REG = (REG >> BIT_NUM)|(REG << (8 - BIT_NUM)))

#define ROTATE_BIT_L(REG,BIT_NUM)  (REG = (REG << BIT_NUM)|(REG >> (8 - BIT_NUM)))

#define CHECK_SET(REG,BIT_NUM)     (REG & (1 << BIT_NUM))

#define CHECK_CLEAR(REG,BIT_NUM)   (!(REG & (1 << BIT_NUM)))

#endif /* MACROS_H_ */
