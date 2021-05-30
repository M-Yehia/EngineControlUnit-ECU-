/*-------------------------------------------------------------------------
 - File Name: external_eeprom.h
 -
 - Module: External EEPROM
 -
 - Description: Header file for the External EEPROM memory using TWI(I2C)
 -
 - Author: Mohamed Yehia
 -------------------------------------------------------------------------*/

#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "macros.h"
#include "typedef.h"
#include "micro_config.h"
#include "TWI.h"

/*-------------------------------------------------------------------------
                               EEPROM MACROS
 -------------------------------------------------------------------------*/

#define ERROR 0
#define SUCCESS 1
#define EEPROM_FIXED_ADDRESS 0xA0

/*-------------------------------------------------------------------------
                           Functions Prototypes
 -------------------------------------------------------------------------*/

void EEPROM_init (void);

uint8 EEPROM_writeByte (uint16 u16add, uint8 u8data);

uint8 EEPROM_readByte(uint16 u16add, uint8 *u8data);

#endif /* EXTERNAL_EEPROM_H_ */
