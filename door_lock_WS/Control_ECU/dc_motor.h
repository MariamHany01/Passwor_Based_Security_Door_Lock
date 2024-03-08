/*
 * dc_motor.h
 *
 *  Created on: 12 Oct 2023
 *      Author:Marim Hany
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

#define INPUT1_PORT_ID PORTB_ID
#define INPUT1_PIN_ID  PIN4_ID

#define INPUT2_PORT_ID  PORTB_ID
#define INPUT2_PIN_ID   PIN5_ID

#define ENABLE_PORT_ID  PORTB_ID
#define ENABLE_PIN_ID   PIN3_ID

/* enum to know thw state of rotTion */
typedef enum {
	STOP,
	CW,
	Anti_CW,

}DcMotor_State;

/***********************************************************************************************
                             important fn prototypes
 ************************************************************************************************/
/*
 * description :
 * setup the ports direction of 2 motor pins input1 and input2 stop the motor at first
 */
void DcMotor_init(void );

/*description:
 fn responsible for rotating the motor CE/anti CW or stop it based on the state of also send the
 required duty cycle to the pwm driver based on the required speen value  */
void DcMotor_Rotate (DcMotor_State state ,uint8 speed );

/*needed fn to set the duty cycle */
void PWM_Timer0_Init(unsigned char set_duty_cycle);

#endif /* DC_MOTOR_H_ */
