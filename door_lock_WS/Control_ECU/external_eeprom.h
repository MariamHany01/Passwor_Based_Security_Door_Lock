 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.h
 *
 * Description: Header file for the External EEPROM Memory
 *
 * Author: Mariam Hany Nabil
 *
 *******************************************************************************/


#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/* write byte fn takes address from TWI (11 bits) and data will be (1 byte ) and returns
 * success or fail */
uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data);

/* read byte fn takes address from TWI byte you want to read from (11 bits) and pointer to
  address data variable where I can store data in to and return success or fail  */
uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data);
 
#endif /* EXTERNAL_EEPROM_H_ */
