/*-------------------------------------------------------------------------
 - File Name: UART.h
 -
 - Module: UART0,UART1
 -
 - Description: Header file for the UART driver for AtMega128
 -
 - Author: Mohamed Yehia
 -------------------------------------------------------------------------*/

#ifndef UART_H_
#define UART_H_


#include "macros.h"
#include "typedef.h"
#include "micro_config.h"


/*-------------------------------------------------------------------------
                            Types Declaration
 -------------------------------------------------------------------------*/

typedef enum{
	NO_PARITY,EVEN_PARITY = 2, ODD_PARITY
}UART_ParityMode;

typedef enum{
	_1_bit,_2_bit
}UART_StopBitSelect;

typedef enum{
	_5_bit,_6_bit,_7_bit,_8_bit,_9_bit=7
}UART_DataSize;

typedef enum{
    BR2400 = 2400,BR4800 = 4800,BR9600 = 9600, BR14400=14400,
	BR19200 = 19200,BR28800=28800,BR38400 = 38400,BR57600 = 57600,
	BR76800=76800,BR115200 = 115200,BR230400=230400, BR250000=250000,
	BR500000=500000, BR1M=1000000
}UART_BaudRate;

typedef struct{
	UART_ParityMode    parity_mode;
	UART_StopBitSelect stop_bit;
	UART_DataSize      data_size;
	UART_BaudRate      baud_rate;
}UART_ConfigType;
/*-------------------------------------------------------------------------
                      Functions UART0 Prototypes
 -------------------------------------------------------------------------*/

void UART0_init (const UART_ConfigType *);

void UART0_sendByte (const uint8);

uint8 UART0_receiveByte (void);

void UART0_sendString (const uint8 *);

void UART0_receiveString (uint8 *);

/*-------------------------------------------------------------------------
                     Functions UART1 Prototypes
 -------------------------------------------------------------------------*/

void UART1_init (const UART_ConfigType *);

void UART1_sendByte (const uint8);

uint8 UART1_receiveByte (void);

void UART1_sendString (const uint8 *);

void UART1_receiveString (uint8 *);

#endif /* UART_H_ */
