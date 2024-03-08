/*
 * hmi.ecu.h
 *
 *  Created on: 2 Nov 2023
 *      Author: Mariam Hany
 */

#ifndef HMI_ECU_H_
#define HMI_ECU_H_

#include "std_types.h"


/*************************************************************************************************
  	  	  	  	  	  	  		important macros for the applicatiom
 ************************************************************************************************/
/* define a ready byte for each micro so that no overlapping takes place */
#define HMI_ECU_READY 0XFF
#define CONTROL_ECU_READY 0X00


#define PASS_LEN 5	/* length of the password */
#define ENTER_BUTON	13 /* define the enter button in this application to be button 13 (on/c)btn */

/* define the next step excuted */
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
/* function that create pass no input or output*/
void CreatePass(void);

/*Description :
 * fn that to take pass from the user and pass it to mc2*/
void EnterPass (void);



#endif /* HMI_ECU_H_ */
