/*
 * buzzer.h
 *
 *  Created on: 26 Oct 2023
 *      Author: Marim Hany
 */

#ifndef BUZZER_H_
#define BUZZER_H_
#include"std_types.h"
/************************************************************************************************
 	 	 	 	 for configurations
 *********************************************************************************************/
#define BUZZER_PORT_ID PORTA_ID
#define BUZZER_PIN_ID  PIN0_ID
/*set the direction of buzzer pin as output using gpio driver */
void Buzzer_init(void);


void Buzzer_on(void);

void Buzzer_off(void);

#endif /* BUZZER_H_ */
