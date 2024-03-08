/*
 * keypad.h
 *
 *  Created on: 29 Sep 2023
 *      Author: Mariam Hany
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include"std_types.h"

     /* important definitions */


/* configuration for number of rows and cols */
#define KEYPAD_NUM_COLS 4
#define KEYPAD_NUM_ROWS 4

/*CONFIGURATION FOR PORT AND START PIN FOR BOTHS COLS AND ROWS */
#define KEYPAD_ROW_PORT_ID               PORTA_ID
#define KEYPAD_FIRST_ROW_PIN_ID          PIN0_ID

#define KEYPAD_COL_PORT_ID               PORTA_ID
#define KEYPAD_FIRST_COL_PIN_ID          PIN4_ID

/*keypad logic configuration
 * pull_up->pressed:logic low
 * pull_down->pressed:logic high
 */
#define KEYPAD_BUTTON_PRESSED            LOGIC_LOW
#define KEYPAD_BUTTON_RELEASED           LOGIC_HIGH

/* Functions Prototypes */
/*
 * Description :
 * Get the Keypad pressed button
 */
uint8 KEYPAD_getPressedKey(void);
#endif /* KEYPAD_H_ */
