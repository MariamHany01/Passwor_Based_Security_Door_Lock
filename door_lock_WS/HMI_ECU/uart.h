 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mariam Hany
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
/**************************************************************************************************
 	 	 	 	 	 	 	 important datatypes
************************************************************************************************ */
/*define a special character to act the the terminator of the string to kniw that the
string has terminated */
#define UART_SPECIAL_CHARACTER_TERMINATOT '#'
/*define number of data bits transmitted or received */
typedef enum {
	FIVE_BIT,
	SIX_BIT,
	SEVEN_BIT,
	EIGHT_BIT,
	NINE_BIT=7
}UART_BitData;

/*select the parity checker mode */
typedef enum {
	DISABLED,
	RESERVED,
	ENABLE_EVEN,
	ENABLE_ODD
}UART_Parity;
/*Select number of stop bits will be used in the frame normally 1 bit */
typedef enum {
	ONE_BIT,
	TWO_BITS
}UART_StoptBit;


typedef struct {
	UART_BitData bit_data;
	UART_Parity parity;
	UART_StoptBit stop_bit;
	uint32 UART_BaudRate ;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the UART_SPECIAL_CHARACTER_TERMINATOT through UART from
 * the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
