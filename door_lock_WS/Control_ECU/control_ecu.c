/*
 * control_ecu.c
 *
 *  Created on: 2 Nov 2023
 *      Author: Marim Hany
 */
/***************************************************************************
				important includes
 ****************************************************************************/
#include "buzzer.h"
#include"dc_motor.h"
#include"external_eeprom.h"
#include"timer1.h"
#include "uart.h"
#include "std_types.h"
#include"control_ecu.h"
#include "twi.h"
#include "util/delay.h"
#include "avr/io.h"

/***************************************************************************
 	 	 	 	 Global variables
 ***************************************************************************/

STEP g_step=SET_PASS; /* variable to know which step you re going to do */

volatile static uint8 g_tick=0; /*global variables to count timer ticks*/
/****************************************************************
 	 	 	 private functions prototype
 ****************************************************************/
static void opeen_door_motor_state_timer_control(void);
static void freezeApp(void);
/*************************************************************************
 	 	 	 	 	 	function definition
 *************************************************************************/
int main (void)
{uint8 state ;/*var to store in  fail or pass after each step to check the state excuted step  */
uint8 i=0;/* var to loop with on the trials of entering pass */
uint8 choice; /*to store in wheather to open the door or change pass*/

UART_ConfigType uart1={EIGHT_BIT,DISABLED,ONE_BIT,9600};/* set the configurations of the uart */
UART_init(&uart1);
Timer1_ConfigTypes timer={0,23500,CLK_1024,CTC };/* set the timer config*/

TWI_ConfigType twi1={1,400000};/* set twi config */
TWI_init(&twi1);
Buzzer_init();
SREG |=(1<<7);/* enable the I_bit */


while(1){
	switch (g_step){

	case SET_PASS:
		/* FIRST: create pass and wait until state= pass means that the pass is set */
		state =CreatePass();
		if  (state!=PASS){
			g_step=SET_PASS;
		}
		else {
			g_step=MAIN_OPTION;
		}
		/* send the step to HMI _ecu */
		UART_sendByte(CONTROL_ECU_READY);
		UART_sendByte(g_step);
		break;

	case MAIN_OPTION:
		/* recieve the choics (open the door /change pass ) from hmi */
		while (UART_recieveByte()!=HMI_ECU_READY);
		choice=UART_recieveByte();
		g_step=ENTER_PASS;/* both cases you need to enter pass */

		break;

	case ENTER_PASS:

		state=EnterPass();
		i++;/* entered trials of pass */
		if (state!=PASS && i<3){/* wrong pass with trials <3*/
			g_step=ENTER_PASS;

		}
		else if (state==PASS){/* right pass */
			i=0;
			g_step=choice;
		}
		else if (state!=PASS && i>=3){/* wrong pass with trials >=3*/
			i=0;
			g_step=WRONG_PASS;
		}
		/* send the next step to hmi */
		UART_sendByte(CONTROL_ECU_READY);
		UART_sendByte(g_step);
		break;

	case OPEN_THE_DOOR:
		/* calculate the timer tick:
		 * Ftimer=Fcpu/N=8MG/N
		 * Ttimer=1/Ftimer=N/8MG
		 * Tcmp=cmp_val*Timer=cmp_val*(N/8MG)=3 sec
		 * assume N=1024 ->cmp_val=23500
		 */

		DcMotor_init();


		Timer1_SetCallBack(&opeen_door_motor_state_timer_control);
		Timer1_init(&timer);
		DcMotor_Rotate(CW,100);
		_delay_ms(500);

		g_step=ERROR_FREEZ;

		break;

	case WRONG_PASS:
		Buzzer_on();
		Timer1_SetCallBack(freezeApp);
		timer.compare_value=47000;
		Timer1_init(&timer);
		g_step=ERROR_FREEZ;


		break;

	case ERROR_FREEZ:
		/* do nothing except intrrupt routine */
		break;
	}

}

}



/************************************************************************************************
 	 	 	 	 	 	 functions definitions
 ***********************************************************************************************/
/* function receives the 2 pass from mc1 and check if they are the same or not
 * input  is nothing and return pass or fail
 * pass:to entered pass are the same and the pass is set and stored in eeprom
 * fail:2 entered pass are not the same and the pass is not set
 */
uint8 CreatePass(void){
	uint8 state=PASS ;/* its value is pass or fail */
	uint8 i;/* var to loop with over pas length */
	uint8 pass[PASS_LEN];/*array to store in the pass */
	uint8 rentered_pass[PASS_LEN];/*array to store in the pass

		 	 	 	 	 	 	 	 	 taken for the 2nd time for approvement  */
	while (UART_recieveByte()!=HMI_ECU_READY);
	/*take the first entered pass from the user */
	for (i=0;i<PASS_LEN;i++){
		pass[i]=UART_recieveByte();
	}
	while (UART_recieveByte()!=HMI_ECU_READY);
	/*take the second entered pass from the user */
	for (i=0;i<PASS_LEN;i++){
		rentered_pass[i]=UART_recieveByte();
	}
	/*check if the2 pass are the same or not */
	for(i=0;i<PASS_LEN;i++){
		if (pass[i]!=rentered_pass[i]){
			state= FAIL;
			break;
		}
	}
	if (state !=FAIL){
		/* store the pass in eeprom  */
		for(i=0;i<PASS_LEN;i++){
			EEPROM_writeByte(0x0310+i, pass[i]); /* Write 0x0F in the external EEPROM */
			_delay_ms(10);
		}

	}
	return state ;
}
/*Description :
 * fn that to take pass from mc1 through the uart and check if it is the same as that
 * pass stores in eeprom
 * input is nothing and output is pass/fail
 * pass: pass matches / fail :pass does not matched
 */
uint8 EnterPass (void){
	uint8 state=PASS ;
	uint8 i;/* variable to loop with over the len of pass */
	uint8 pass[PASS_LEN];/*array to store in the pass get from mc1*/
	uint8 val;/* variable store in the readed value from eeprom */


	while (UART_recieveByte()!=HMI_ECU_READY);

	/*take the  entered pass from the user */
	for (i=0;i<PASS_LEN;i++){
		pass[i]=UART_recieveByte();
	}
	/* check if the entered pass matches the pass stored in eeprom or not */
	for (i=0;i<PASS_LEN;i++){
		EEPROM_readByte(0x0310+i, &val);
		if (val!=pass[i]){
			state=FAIL;
			break;
		}
	}
	return state ;
}


/*description :
 * rotate the dc motor CW for 15 sec then then hold the motor for 3 sec
 * then rotate anti_CK for 15 sec
 */
void opeen_door_motor_state_timer_control(void){
	/* increment the tick every time 1tick=3sec */

	g_tick++;
	if (g_tick==5){/* 15 sec passed */
		DcMotor_Rotate(STOP,0);
	}
	else if (g_tick==6){/* 18 sec passed */
		DcMotor_Rotate(Anti_CW,100);
	}
	else if (g_tick==11){ /*33 sec passed */
		DcMotor_Rotate(STOP,0);
		g_tick=0;
		Timer1_deInit();

		g_step=MAIN_OPTION;

	}
}
/*function to freeze the app and turn on the buzzer for 1 min */
void freezeApp(void){
	/* increment the ticks each time 1tick=6sec */
	g_tick++;
	if (g_tick==10){/*1 min passed */
		Buzzer_off();
		Timer1_deInit();
		g_tick=0;
		g_step=MAIN_OPTION;
	}
}
