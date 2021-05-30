/*-------------------------------------------------------------------------
 - File Name: TWI.c
 -
 - Module: TWI (I2C)
 -
 - Description: Source file for the TWI(I2C) AVR driver
 -
 - Author: Mohamed Yehia
 -------------------------------------------------------------------------*/

#include "TWI.h"

/*-------------------------------------------------------------------------
                                Functions
 -------------------------------------------------------------------------*/
         /*------------------- Description --------------------
          -  Function to initialize TWI(I2C) Module
          -  1. TWBR = Calculated value to specific rate and
                prescaler
          -  2. TWCR: Controls the enable of Interrupt (TWIE)
                      Controls the enable of the Module(TWEN)
          -  3. TWAR: Set the module address if any master
                device want to call it
          ----------------------------------------------------*/
void TWI_init (const TWI_Config *config_ptr)
{
    /*Bit Rate register equation*/
	TWBR = (F_CPU - 16*(config_ptr -> S_clock))/(2*(4^config_ptr -> Prescaler)*(config_ptr -> S_clock));

	/* Interrupt Control*/
	CLEAR_BIT (TWCR,TWIE);
	TWCR |= ((config_ptr -> Interrupt) << TWIE);

	/* Set my address if any master device want to call me
	 * TWGCE: Control General Call Recognition bit */
	TWAR = ((config_ptr -> MySlaveAddress) << TWA0) | ((config_ptr -> GCRecognitionEnable) << TWGCE);

	/* Enable TWI Module*/
	SET_BIT (TWCR,TWEN);
}

         /*------------------- Description --------------------
                       Function to send start bit
          ----------------------------------------------------*/
void TWI_start (void)
{
	/* 1. TWEN=1 To enable the TWI module
	 * 2. TWINT=1 Flag must be cleared by software by writing logic one
	 * 3. TWSTA=1 Become a master and send start bit */
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);

    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(CHECK_CLEAR(TWCR,TWINT));
}

         /*------------------- Description --------------------
                       Function to stop start bit
          ----------------------------------------------------*/
void TWI_stop (void)
{
	/* 1. TWEN=1 To enable the TWI module
	 * 2. TWINT=1 Flag must be cleared by software by writing logic one
	 * 3. TWSTO=1 send Stop bit */
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);
}

         /*------------------- Description --------------------
                          Function to send data
          ----------------------------------------------------*/
void TWI_write (uint8 data)
{
    /* Save the data in TWI data Register */
    TWDR = data;

    /* 1. TWINT=1 Flag must be cleared by software by writing logic one
	 *            before sending the data
	 * 2. TWEN=1 Enable TWI Module */
    TWCR = (1 << TWINT) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
    while(CHECK_CLEAR (TWCR,TWINT));
}

         /*------------------- Description --------------------
                 Function to read data and send ACK bit
          ----------------------------------------------------*/
uint8 TWI_readWithACK(void)
{
	/* 1. TWINT=1 Flag must be cleared by software by writing logic one
	 *            before reading the data
	 * 2. TWEA=1 Enable sending ACK after reading or receiving data
	 * 3. TWEN=1 Enable TWI Module */
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(CHECK_CLEAR (TWCR,TWINT));

    /* Read Data */
    return TWDR;
}

         /*------------------- Description --------------------
                Function to read data and without ACK bit
          ----------------------------------------------------*/
uint8 TWI_readWithNACK(void)
{
	/* 1. TWINT=1 Flag must be cleared by software by writing logic one
	 *            before reading the data
	 * 2. TWEN=1 Enable TWI Module */
    TWCR = (1 << TWINT) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(CHECK_CLEAR (TWCR,TWINT));

    /* Read Data */
    return TWDR;
}

uint8 TWI_getStatus(void)
{
    uint8 status;
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWSR & 0xF8;
    return status;
}

