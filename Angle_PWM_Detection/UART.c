/*-------------------------------------------------------------------------
 - File Name: UART.c
 -
 - Module: UART0,UART1
 -
 - Description: Source file for the UART driver for AtMega128
 -
 - Author: Mohamed Yehia
 -------------------------------------------------------------------------*/

#include "UART.h"

/*-------------------------------------------------------------------------
                          Functions of UART1
 -------------------------------------------------------------------------*/
         /*------------------- Description --------------------
          -  Function to initialize the UART0 driver
          -  1. Select parity mode.
          -  2. Select stop bit.
          -  3. Select data size.
          -  4. Set baud rate.
          ----------------------------------------------------*/
void UART0_init (const UART_ConfigType *config_ptr)
{
	/* U2X0 = 1 for double transmission speed for Asynchronous */
	UCSR0A = (1<<U2X0);
	/*------------------- UCSRB Description --------------------
	 - RXCIE0 = 0 Disable RX Complete Interrupt Enable
	 - TXCIE0 = 0 Disable TX Complete Interrupt Enable
	 - UDRIE0 = 0 Disable Data Register Empty Interrupt Enable
	 - RXEN0  = 1 Receiver Enable
	 - TXEN0  = 1 Transmitter Enable
	 - UCSZ02 & RXB80 & TXB80 Used for 9-bit data mode
     ----------------------------------------------------------*/
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);

	if ((config_ptr -> data_size) == 7)
	{
	UCSR0B |= (1<<UCSZ02) | (1<<RXB80) | (1<<TXB80);
	}

	/*------------------- UCSRC Description --------------------
	 - UMSEL   = 0 Asynchronous Operation
	 - UPM01:0    Parity Mode Selected
	 - USB0S      Stop Bit Selected
	 - UCPOL0   = 0 (Used with the Synchronous operation only)
	 - UCSZ01:0   Data Size Selected
     ----------------------------------------------------------*/
	UCSR0C = ((config_ptr -> parity_mode)  <<UPM00) |
			 ((config_ptr -> stop_bit)     <<USBS0) |
			 (((config_ptr -> data_size)&3) <<UCSZ00);

	/*Equation for calculating UBRR depending on baud rate
	 - UBRR0L for the least 8-bits
	 - UBBR0H for the most 4-bits
	 */
	UBRR0L = (((F_CPU/((config_ptr -> baud_rate) * 8))-1));
	UBRR0H = ((((F_CPU/((config_ptr -> baud_rate) * 8))-1)) >> 8);
}

         /*------------------- Description --------------------
          -  Function to send a byte
          -  1. Receive desired byte to be sent
          -  2. Use Polling method
          ----------------------------------------------------*/
void UART0_sendByte (const uint8 data)
{
	/* UDRE0 is 1 when the buffer is empty and ready to receive
	 * new data, So wait until the flag is 1*/
	while (CHECK_CLEAR(UCSR0A,UDRE0)){}
	/* Write data into UDR register */
	UDR0 = data;
}

         /*------------------- Description --------------------
          -  Function to receive a byte
          -  1. Return data into UDR
          -  2. Use Polling method
          ----------------------------------------------------*/
uint8 UART0_receiveByte (void)
{
	/* RXC0 is 1 when there are unread in a receive buffer
	 * (Receive Completed), So wait until the flag is 1*/
	while (CHECK_CLEAR(UCSR0A,RXC0)){}
	/* return data from UDR register */
	return UDR0;
}

         /*------------------- Description --------------------
           -  Function to send a String
           -  1. Receive desired string to be sent into pointer
           ----------------------------------------------------*/
void UART0_sendString (const uint8 *str)
{
	while (*str != '\0')
	{
		UART0_sendByte(*str);
		str++;
	}
}

         /*------------------- Description --------------------
         -  Function to receive a String
         -  1. Receive desired string to be sent into pointer
         -  2. Use Polling method
         ----------------------------------------------------*/
void UART0_receiveString (uint8 *str)
{
	str = UART0_receiveByte;
	while (*str != '#')
	{
		str++;
		str = UART0_receiveByte;
	}
	str = '\0';
}

/*-------------------------------------------------------------------------
                          Functions of UART1
 -------------------------------------------------------------------------*/
         /*------------------- Description --------------------
          -  Function to initialize the UART1 driver
          -  1. Select parity mode.
          -  2. Select stop bit.
          -  3. Select data size.
          -  4. Set baud rate.
          ----------------------------------------------------*/
void UART1_init (const UART_ConfigType *config_ptr)
{
	/* U2X1 = 1 for double transmission speed for Asynchronous */
	UCSR1A = (1<<U2X1);
	/*------------------- UCSRB Description --------------------
	 - RXCIE1 = 0 Disable RX Complete Interrupt Enable
	 - TXCIE1 = 0 Disable TX Complete Interrupt Enable
	 - UDRIE1 = 0 Disable Data Register Empty Interrupt Enable
	 - RXEN1  = 1 Receiver Enable
	 - TXEN1  = 1 Transmitter Enable
	 - UCSZ12 & RXB81 & TXB81 Used for 9-bit data mode
     ----------------------------------------------------------*/
	UCSR1B = (1<<RXEN1) | (1<<TXEN1);

	if ((config_ptr -> data_size) == 7)
	{
	UCSR1B |= (1<<UCSZ12) | (1<<RXB81) | (1<<TXB81);
	}

	/*------------------- UCSRC Description --------------------
	 - UMSEL1   = 0 Asynchronous Operation
	 - UPM11:0    Parity Mode Selected
	 - USB1S      Stop Bit Selected
	 - UCPOL1   = 0 (Used with the Synchronous operation only)
	 - UCSZ11:0   Data Size Selected
     ----------------------------------------------------------*/
	UCSR0C = ((config_ptr -> parity_mode)  <<UPM10) |
			 ((config_ptr -> stop_bit)     <<USBS1) |
			 (((config_ptr -> data_size)&3) <<UCSZ10);

	/*Equation for calculating UBRR depending on baud rate
	 - UBRR1L for the least 8-bits
	 - UBBR1H for the most 4-bits
	 */
	UBRR1L = (((F_CPU/((config_ptr -> baud_rate) * 8))-1));
	UBRR1H = ((((F_CPU/((config_ptr -> baud_rate) * 8))-1)) >> 8);
}

         /*------------------- Description --------------------
          -  Function to send a byte
          -  1. Receive desired byte to be sent
          -  2. Use Polling method
          ----------------------------------------------------*/
void UART1_sendByte (const uint8 data)
{
	/* UDRE1 is 1 when the buffer is empty and ready to receive
	 * new data, So wait until the flag is 1*/
	while (CHECK_CLEAR(UCSR1A,UDRE1)){}
	/* Write data into UDR register */
	UDR1 = data;
}

         /*------------------- Description --------------------
          -  Function to receive a byte
          -  1. Return data into UDR
          -  2. Use Polling method
          ----------------------------------------------------*/
uint8 UART1_receiveByte (void)
{
	/* RXC1 is 1 when there are unread in a receive buffer
	 * (Receive Completed), So wait until the flag is 1*/
	while (CHECK_CLEAR(UCSR1A,RXC1)){}
	/* return data from UDR register */
	return UDR1;
}

         /*------------------- Description --------------------
           -  Function to send a String
           -  1. Receive desired string to be sent into pointer
           ----------------------------------------------------*/
void UART1_sendString (const uint8 *str)
{
	while (*str != '\0')
	{
		UART1_sendByte(*str);
		str++;
	}
}

         /*------------------- Description --------------------
         -  Function to receive a String
         -  1. Receive desired string to be sent into pointer
         -  2. Use Polling method
         ----------------------------------------------------*/
void UART1_receiveString (uint8 *str)
{
	str = UART1_receiveByte;
	while (*str != '#')
	{
		str++;
		str = UART1_receiveByte;
	}
	str = '\0';
}
