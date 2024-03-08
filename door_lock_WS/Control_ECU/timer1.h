/*
 * timer1.h
 *timer1 driver that supports compare and normal mood only
 *  Created on: 28 Oct 2023
 *  Author: Mariam Hany
 */

#ifndef TIMER1_H_
#define TIMER1_H_
#include "std_types.h"
/************************************************************************************************
 	 	 	 	 	 	important structs/enums
 ***********************************************************************************************/
typedef enum {
	NO_CLK,
	CLK_1,
	CLK_8,
	CLK_64,
	CLK_256,
	CLK_1024
}Timer1_Prescaler;
typedef enum {
	NORMAL,
	CTC =4
}Timer1_Mode;
typedef struct {
	uint16 initial_value ;
	uint16 compare_value ;/*will be used in compare mood only */
	Timer1_Prescaler prescaler ;
	Timer1_Mode mode ;
}Timer1_ConfigTypes;
/**************************************************************************************************
  	  	  	  	  	  	  	  	  Function Prototypes
********************************************************************************************** */

/* set the configurations of timer1 and enable it by setting clk */
void Timer1_init(const Timer1_ConfigTypes *Config_Ptr);
/* reset all config and disable the clk */
void Timer1_deInit(void);

void Timer1_SetCallBack(void(*a_ptr(void)));


#endif /* TIMER1_H_ */
