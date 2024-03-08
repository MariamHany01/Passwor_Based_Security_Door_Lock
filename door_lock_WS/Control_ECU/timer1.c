/*
 * timer1.c
 *timer1 driver that supports compare and normal mood only
 *  Created on: 28 Oct 2023
 *  Author: Mariam Hany
 */
#include "timer1.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "common_macros.h"

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;
/*******************************************************************************************************************
						functions definition
 *****************************************************************************************************************/
/* set the configurations of timer1 and enable it by setting clk */
void Timer1_init(const Timer1_ConfigTypes *Config_Ptr){
	/*TCCR1A REG:
	 * 1.FOC1A&FOC1B are always set :non PWM mode
	 * 2.WGM11&WGM10=Config_Ptr->mode(LSB)
	 */
	TCCR1A =(1<<FOC1A)|(1<<FOC1B);/*non_pwm mode */
	TCCR1A=(TCCR1A&0XFC)|((Config_Ptr->mode)&0X03);/*insert the LSB of the mood config */


	/* start count from TCNT1 REG_VALUE */
	TCNT1= Config_Ptr->initial_value;

	if (Config_Ptr->mode==CTC){
		OCR1A=Config_Ptr->compare_value;/* set the compare value */
		SET_BIT(TIMSK,OCIE1A);
	}
	else {

		 SET_BIT(TIMSK,TOIE1);
	}
	/*TCCR1B REG :
	 * WGM13&WGM12=Config_Ptr->mode(MSB)
	 * CS12&CS11&CS10=Config_Ptr->prescaler
	 */
	TCCR1B = (TCCR1B & 0xE7) | ((Config_Ptr->mode & 0x0C)  << 1);
//	TCCR1B =(1<<WGM12);
	TCCR1B=(TCCR1B&0XF8)|(Config_Ptr->prescaler);

}
/* reset all config and disable the clk */
void Timer1_deInit(void){
	TCCR1A=0;
	TCCR1B=0;
	TCNT1=0;
	OCR1A=0;
	TIMSK=0;

}

void Timer1_SetCallBack( void(*a_ptr(void))){
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}
ISR( TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
ISR( TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

