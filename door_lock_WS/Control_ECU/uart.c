 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author:Marim Hany
 *
 *******************************************************************************/

#include "uart.h"
#include "avr/io.h" /* To use the UART Registers */
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <avr/interrupt.h>

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_ptr)
{
	uint16 ubrr_value = 0;

   /* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);

	/************************** UCSRB Description **************************
	 * RXCIE = 0 enable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ0,UCSZ1,UCSZ2 =Config_ptr->bit_data
	 *if Config_ptr->bit_data==9..... RXB8 & TXB8=1,1
	 ***********************************************************************/ 
	UCSRB = (1<<RXEN) | (1<<TXEN);
	/*insert the bit_data bit_2 in bit2 inUCSRB and bit0,1 in UCSRC */
	UCSRB =(UCSRB&0xFB)|(Config_ptr->bit_data&0X04);


	if (Config_ptr->bit_data==NINE_BIT){
		SET_BIT(UCSRB,RXB8);
		SET_BIT(UCSRB,TXB8);
	}

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = Config_ptr->parity
	 * USBS    = Config_ptr->stop_bit
	 * UCSZ1:0 = Config_ptr->bit_data&0X03 to insert UCSZ0and UCSZ1
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/



	UCSRC=(UCSRC&0X4F)|(Config_ptr->parity&0X83) ;
	UCSRC=(UCSRC&0X7B)|(Config_ptr->stop_bit%0X81);
	UCSRC =(UCSRB&0x79)|(Config_ptr->bit_data&0X83);

	
	/* Calculate the UBRR register value */
	ubrr_value = (uint16)(((F_CPU / ((Config_ptr->UART_BaudRate) * 8UL))) - 1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;
}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
    return UDR;		
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}

}

/*
 * Description :
 * Receive the required string until the UART_SPECIAL_CHARACTER_TERMINATOT through UART from
 * the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != UART_SPECIAL_CHARACTER_TERMINATOT)
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}
