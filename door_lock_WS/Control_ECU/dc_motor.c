/*
 * dc_motor.c
 *
 *  Created on: 12 Oct 2023
 *      Author:Marim Hany
 */


/*
 * description :
 * setup the ports direction of 2 motor pins input1 and input2 stop the motor at first
 */
#include "gpio.h"
#include"common_macros.h"
#include <avr/io.h>
#include "dc_motor.h"
void DcMotor_init(void ){
	/* set the pin of input 1 as output*/
	GPIO_setupPinDirection(INPUT1_PORT_ID,INPUT1_PIN_ID,PIN_OUTPUT);

	/* set the pin of input 2 as output*/
	GPIO_setupPinDirection(INPUT2_PORT_ID,INPUT2_PIN_ID,PIN_OUTPUT);

	/* motor initially stoped */
	GPIO_writePin(INPUT1_PORT_ID,INPUT1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(INPUT2_PORT_ID,INPUT2_PIN_ID,LOGIC_LOW);
}
/*description:
 fn responsible for rotating the motor CE/anti CW or stop it based on the state of also send the
 required duty cycle to the pwm driver based on the required speen value  */
void DcMotor_Rotate (DcMotor_State state ,uint8 speed ){
	/*initiate pwm to support dc motor with given duty cycle */
	PWM_Timer0_Init(speed);
	switch (state){
	case STOP:
		GPIO_writePin(INPUT1_PORT_ID,INPUT1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(INPUT2_PORT_ID,INPUT2_PIN_ID,LOGIC_LOW);
		break;
	case CW:
		GPIO_writePin(INPUT1_PORT_ID,INPUT1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(INPUT2_PORT_ID,INPUT2_PIN_ID,LOGIC_HIGH);
		break;
	case Anti_CW:
		GPIO_writePin(INPUT1_PORT_ID,INPUT1_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(INPUT2_PORT_ID,INPUT2_PIN_ID,LOGIC_LOW);
		break;

	}
}
void PWM_Timer0_Init(unsigned char set_duty_cycle)
{

	TCNT0 = 0; //Set Timer Initial value

	OCR0  = set_duty_cycle; // Set Compare Value

	GPIO_setupPinDirection(ENABLE_PORT_ID,ENABLE_PIN_ID,PIN_OUTPUT); //set enable pin  as output pin --> pin where the PWM signal is generated from MC.

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}
