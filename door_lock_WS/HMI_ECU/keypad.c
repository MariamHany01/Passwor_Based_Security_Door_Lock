/*
 * keypad.c
 *
 *  Created on: 29 Sep 2023
 *      Author: Mariam Hany
 */
#include"common_macros.h"
#include"gpio.h"
#include<util/delay.h>
#include"keypad.h"

     /*Prototypes of private functions */
#ifndef STANDARD_KEYPAD

#if (KEYPAD_NUM_COLS==3)
/* a fn that take number of button and return the corresponding value */
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number);

#elif (KEYPAD_NUM_COLS==4)
/* a fn that take number of button and return the corresponding value */
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number);

#endif
#endif

/* function definition */
/* Description :
* Get the Keypad pressed button
*/
uint8 KEYPAD_getPressedKey(void){
	uint8 row,col; /* variables to itterate over the rows and cols */

	/* setting all rows direction to input direction */
	for (row=0;row<4;row++){
	 GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID +row ,PIN_INPUT);
	}

	/*set the first 3 cols (as for sure the will be set in both cases 4*4 and 4*3 )to input  */
	for (col=0;col<3;col++){
		GPIO_setupPinDirection(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID +col ,PIN_INPUT);
	}

	/* in case of 4*4 keypad adjust the last pin in cols */
	#if(KEYPAD_NUM_COLS==4)
		GPIO_setupPinDirection(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID +col ,PIN_INPUT);
	#endif

/*the main algorithm of keypad:
 * first: set a row in a grand loop as output pin and set it to logic high or low (pressed logic)
 * second :loop in a minor loop to get the col on which a logic low/high is on it(pressed logic)
 * third:call another fn to adjust the output(in case of non standard keypad
 */

while(1){

	for (row=0;row<KEYPAD_NUM_ROWS;row++){
		/*set the row as outpus  */
		GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID +row ,PIN_OUTPUT);

		/*write logic pressed on that row */
		GPIO_writePin(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID +row ,KEYPAD_BUTTON_PRESSED);


		/*iterrate over the cols to know which button is pressed in that row(get intersection
		 * between rows and cols)
		 */
		for(col=0;col<KEYPAD_NUM_COLS;col++){
			/*check on col pin if pressed or not */

			if((GPIO_readPin(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID +col))==KEYPAD_BUTTON_PRESSED){
				/*case of 4*3 and standard */
				#if (KEYPAD_NUM_COLS==3)
					#ifdef STANDARD_KEYPAD
				/*case of 4*3 and standard */
						return((row*KEYPAD_NUM_COLS)+col+1);
					else
						/*case of 4*3 and non_standard */
						return KEYPAD_4x3_adjustKeyNumber((row*KEYPAD_NUM_COLS)+col+1);
					#endif
				#elif(KEYPAD_NUM_COLS==4)
					#ifdef STANDARD_KEYPAD
					/*case of 4*4 and standard */
						return((row*KEYPAD_NUM_COLS)+col+1);
					#else
						/*case of 4*4 and non_standard */
						return KEYPAD_4x4_adjustKeyNumber((row*KEYPAD_NUM_COLS)+col+1);
					#endif
				#endif
			}/*end of if cond */
		}/*end of for loop (of cols)*/

		GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID+row,PIN_INPUT);
		_delay_ms(5); /* Add small delay to fix CPU load issue in proteus */
	}/*end of for loop(rows) */
}/*end of while 1 */
}/*end of fn */


#ifndef STANDARD_KEYPAD
	#if (KEYPAD_NUM_COLS==3)
		static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number){
			uint8 button_value =0;
			switch (button_number){
			case 10 :
				button_value='*';
				break;
			case 11:
				button_value=0;
				break;
			case 12:
				button_value ='#';
				break;
			default :
				button_value=button_number;
			}
			return button_value;
		}
#elif(KEYPAD_NUM_COLS==4)
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number){
			uint8 button_value =0;
			switch (button_number){
			case 1 :
				button_value=7;
				break;
			case 2:
				button_value=8;
				break;
			case 3:
				button_value =9;
				break;
			case 4:
				button_value ='%';
				break;
			case 5:
				button_value =4;
				break;
			case 6:
				button_value =5;
				break;
			case 7:
				button_value =6;
				break;
			case 8:
				button_value ='*';
				break;
			case 9:
				button_value =1;
				break;
			case 10:
				button_value =2;
				break;
			case 11:
				button_value =3;
				break;
			case 12:
				button_value ='-';
				break;
			case 13:
				button_value =13;
				break;
			case 14:
				button_value =0;
				break;
			case 15:
				button_value ='=';
				break;
			case 16:
				button_value ='+';
				break;
			default :
				button_value=button_number;
			}
			return button_value;
		}
#endif

#endif
