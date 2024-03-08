/*
 * buzzer.h
 *
 *  Created on: 26 Oct 2023
 *      Author: Marim Hany
 */
#include "gpio.h"
#include "std_types.h"
#include "buzzer.h"


void Buzzer_init(void){

	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID, PIN_OUTPUT);

}
void Buzzer_on(void){
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
}
void Buzzer_off(void){
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}
