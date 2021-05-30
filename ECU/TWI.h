/*-------------------------------------------------------------------------
 - File Name: TWI.h
 -
 - Module: TWI (I2C)
 -
 - Description: Header file for the TWI(I2C) AVR driver
 -
 - Author: Mohamed Yehia
 -------------------------------------------------------------------------*/

#ifndef TWI_H_
#define TWI_H_

#include "macros.h"
#include "typedef.h"
#include "micro_config.h"

/*-------------------------------------------------------------------------
                               TWI MACROS
 -------------------------------------------------------------------------*/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         	0x08 /* start has been sent */
#define TWI_REP_START    	0x10 /* repeated start */

#define TWI_MT_SLA_W_ACK  	0x18 /* Master transmit ( slave address + Write request ) to slave + Ack received from slave */
#define TWI_MT_SLA_W_NACK   0x20 /* Master transmit ( slave address + Write request ) to slave + Not Ack received from slave */
#define TWI_MT_DATA_ACK   	0x28 /* Master transmit ( data ) and ACK has been received from Slave. */
#define TWI_MT_DATA_NACK    0x30 /* Master transmit ( data ) and Not ACK has been received from Slave. */
#define TWI_MT_SLA_R_ACK  	0x40 /* Master transmit ( slave address + Read request ) to slave + Ack received from slave */
#define TWI_MT_SLA_R_NACK   0x48 /* Master transmit ( slave address + Read request ) to slave + Ack received from slave */

#define TWI_MR_DATA_ACK   	0x50 /* Master received ( data ) and send ACK to slave */
#define TWI_MR_DATA_NACK  	0x58 /* Master received ( data ) but doesn't send ACK to slave */

/*-------------------------------------------------------------------------
                            Types Declaration
 -------------------------------------------------------------------------*/
typedef enum{
	PS_1, PS_4, PS_16, PS_64
}TWI_Prescaler;

typedef enum{
	F_100K= 100000, F_400K=400000, F_1M=1000000, F_34M=3400000
}TWI_Clock;

typedef enum{
	Disable, Enable
}TWI_EnDis;

typedef struct{
	TWI_Clock     S_clock;
	TWI_Prescaler Prescaler;
	uint8         MySlaveAddress;
	TWI_EnDis     GCRecognitionEnable;
	TWI_EnDis     Interrupt;
}TWI_Config;

/*-------------------------------------------------------------------------
                           Functions Prototypes
 -------------------------------------------------------------------------*/

void TWI_init (const TWI_Config *config_ptr);

void TWI_start (void);

void TWI_stop (void);

void TWI_write (uint8 data);

uint8 TWI_readWithACK(void);

uint8 TWI_readWithNACK(void);

uint8 TWI_getStatus(void);

#endif /* TWI_H_ */
