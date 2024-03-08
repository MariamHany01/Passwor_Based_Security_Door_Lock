/*
 * control_ecu.h
 *
 *  Created on: 2 Nov 2023
 *      Author: maria
 */

#ifndef CONTROL_ECU_H_
#define CONTROL_ECU_H_

/*************************************************************************************************
  	  	  	  	  	  	  		important macros for the applicatiom
 ************************************************************************************************/
/* define a ready byte for each micro so that no overlapping takes place */
#define HMI_ECU_READY 0XFF
#define CONTROL_ECU_READY 0X00


#define PASS_LEN 5	/* length of the password */


#define PASS 1
#define FAIL 0

typedef enum {
	SET_PASS,
	MAIN_OPTION,
	ENTER_PASS,
	OPEN_THE_DOOR,
	WRONG_PASS,
	ERROR_FREEZ,
}STEP ;

/************************************************************************************************
  	  	  	  	  	  	  	  	  	 functions prototype
************************************************************************************************/
/* function receives the 2 pass from mc1 and check if they are the same or not
 * input  is nothing and return pass or fail
 * pass:to entered pass are the same and the pass is set and stored in eeprom
 * fail:2 entered pass are not the same and the pass is not set
 */
uint8 CreatePass(void);

/*Description :
 * fn that to take pass from mc1 through the uart and check if it is the same as that
 * pass stores in eeprom
 * input is nothing and output is pass/fail
 * pass: pass matches / fail :pass does not matched
 */
uint8 EnterPass (void);



#endif /* CONTROL_ECU_H_ */
