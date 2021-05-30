/*-------------------------------------------------------------------------
 - File Name: external_eeprom.h
 -
 - Module: External EEPROM
 -
 - Description: Source file for the External EEPROM memory using TWI(I2C)
 -
 - Author: Mohamed Yehia
 -------------------------------------------------------------------------*/

#include "external_eeprom.h"

/*-------------------------------------------------------------------------
                                Functions
 -------------------------------------------------------------------------*/
void EEPROM_init (void)
{
	/*Configurations of TWI to suit EEPROM*/
	TWI_Config TWI_config = {F_400K,PS_1,0x01,Disable,Disable};

	/*Initialize TWI(I2C) module*/
	TWI_init (&TWI_config);
}

         /*------------------- Description --------------------
          -  Function to write data in specific address in the
             EEPROM memory
          -  1. Send Start Bit
          -  2. Send the device address and mask to get A8 A9 A10
                address bits and set R/W to 0 (write)
          -  3. Send the required memory location address
                Only Least 8 bits A0:A7
          -  4. Write data to memory
          -  5. Send Stop Bit
          -  6. In every step we check the status to make sure
             that every single step is done and return SUCCESS=1
             if there any error it returns ERROR=0
          ----------------------------------------------------*/
uint8 EEPROM_writeByte (uint16 u16add, uint8 u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_write((uint8)((EEPROM_FIXED_ADDRESS) | ((u16add & 0x0700)>>7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;

    /* Send the required memory location address
     * Only Least 8 bits A0:A7 */
    TWI_write((uint8)(u16add));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    /* Write a data byte to memory */
    TWI_write(u8data);
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();

    return SUCCESS;
}

         /*------------------- Description --------------------
          -  Function to read data from specific address in the
             EEPROM memory
          -  1. Send Start Bit
          -  2. Send the device address and mask to get A8 A9 A10
                address bits and set R/W to 0 (write) to write
                the required address to be read
          -  3. Send the required memory location address
                Only Least 8 bits A0:A7
          -  4. Send the Repeated Start Bit
          -  5. Send the device address and mask to get A8 A9 A10
                address bits and set R/W to 1 (read)
          -  6. Read Byte from Memory without send ACK
          -  7. Send Stop Bit
          -  8. In every step we check the status to make sure
             that every single step is done and return SUCCESS=1
             if there any error it returns ERROR=0
          ----------------------------------------------------*/
uint8 EEPROM_readByte(uint16 u16add, uint8 *u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_write((uint8)((EEPROM_FIXED_ADDRESS) | ((u16add & 0x0700)>>7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;

    /* Send the required memory location address
     * Only Least 8 bits A0:A7 */
    TWI_write((uint8)(u16add));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    /* Send the Repeated Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_REP_START)
        return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=1 (Read) */
    TWI_write((uint8)((EEPROM_FIXED_ADDRESS) | ((u16add & 0x0700)>>7) | 1 /* R/W bit */));
    if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
        return ERROR;

    /* Read Byte from Memory without send ACK */
    *u8data = TWI_readWithNACK();
    if (TWI_getStatus() != TWI_MR_DATA_NACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();

    return SUCCESS;
}
