/**
 ******************************************************************************
 * File:                    Main.c
 * Author:                  David Tolsma
 * Breif Description:       Main function calls
 ******************************************************************************
 * Long Description:
 *
 * Main
 *
 ******************************************************************************
 */

#include "SystemClock.h"
#include "GPIO.h"
#include "pinoutConfiguration.h"
#include <stdio.h>
#include "syscalls.c"
#include "timers.h"

uint32_t counter = 0;

int main(void)
{
	SystemClockInit();
	timersTimer2Init();

	GpioInit();

	while(1){}
}

void gpioOn(void){
	GPIO_SetPin(PP_1_PORT, PP_1_PIN);
	GPIO_SetPin(PP_2_PORT, PP_2_PIN);
	GPIO_SetPin(PP_3_PORT, PP_3_PIN);
	GPIO_SetPin(PP_4_PORT, PP_4_PIN);
	GPIO_SetPin(PP_5_PORT, PP_5_PIN);
	GPIO_SetPin(PP_6_PORT, PP_6_PIN);
	GPIO_SetPin(PP_7_PORT, PP_7_PIN);
	GPIO_SetPin(PP_8_PORT, PP_8_PIN);

	GPIO_SetPin(LSD_1_PORT, LSD_1_PIN);
	GPIO_SetPin(LSD_2_PORT, LSD_2_PIN);
	GPIO_SetPin(LSD_3_PORT, LSD_3_PIN);
	GPIO_SetPin(LSD_4_PORT, LSD_4_PIN);
	GPIO_SetPin(LSD_5_PORT, LSD_5_PIN);
	GPIO_SetPin(LSD_6_PORT, LSD_6_PIN);
	GPIO_SetPin(LSD_7_PORT, LSD_7_PIN);
	GPIO_SetPin(LSD_8_PORT, LSD_8_PIN);

	puts("Turning all outputs on. \n");

}


void gpioOff(void){
	GPIO_ResetPin(PP_1_PORT, PP_1_PIN);
	GPIO_ResetPin(PP_2_PORT, PP_2_PIN);
	GPIO_ResetPin(PP_3_PORT, PP_3_PIN);
	GPIO_ResetPin(PP_4_PORT, PP_4_PIN);
	GPIO_ResetPin(PP_5_PORT, PP_5_PIN);
	GPIO_ResetPin(PP_6_PORT, PP_6_PIN);
	GPIO_ResetPin(PP_7_PORT, PP_7_PIN);
	GPIO_ResetPin(PP_8_PORT, PP_8_PIN);

	GPIO_ResetPin(LSD_1_PORT, LSD_1_PIN);
	GPIO_ResetPin(LSD_2_PORT, LSD_2_PIN);
	GPIO_ResetPin(LSD_3_PORT, LSD_3_PIN);
	GPIO_ResetPin(LSD_4_PORT, LSD_4_PIN);
	GPIO_ResetPin(LSD_5_PORT, LSD_5_PIN);
	GPIO_ResetPin(LSD_6_PORT, LSD_6_PIN);
	GPIO_ResetPin(LSD_7_PORT, LSD_7_PIN);
	GPIO_ResetPin(LSD_8_PORT, LSD_8_PIN);

	printf("Turning all outputs off. \n");

}

typedef enum{
	ST_ON,
	ST_OFF
} State;

void TIM2_IRQHandler(void){
	static State myState = ST_OFF;
	switch(myState){
		case ST_OFF:{
			gpioOn();
			myState = ST_ON;
			break;
		}
		case ST_ON:{
			gpioOff();
			myState = ST_OFF;
			break;
		}
		default:{
			break;
		}
	}
	CLEAR_BIT(TIM2->SR, TIM_SR_UIF);
}

void HardFault_Handler(void)
{
  while(1){}
}
//
