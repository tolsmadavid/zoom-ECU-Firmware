/******************************************************************************
* File:                    Main.c
* Author:                  David Tolsma
* Date Modified:           05/25/2020
* Breif Description:       Main function lives here
*******************************************************************************
* Includes
******************************************************************************/
#include "SystemClock.h"
#include "Gpio.h"
#include "PinoutConfiguration.h"
#include "Tim.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "syscalls.c"

#include <stdio.h>



/******************************************************************************
* Defines
******************************************************************************/


/******************************************************************************
* Public Variables
******************************************************************************/


/******************************************************************************
* Private Function Prototypes (static)
******************************************************************************/
static void testTask(void * pvParameters);
static void gpioOn(void);
static void gpioOff(void);

/******************************************************************************
* Private Variables (static)
******************************************************************************/
typedef enum{
	ST_ON,
	ST_OFF
} state_t;

TaskHandle_t testTaskHandle = NULL;

static state_t myState = ST_OFF;

uint32_t counter = 0;

/******************************************************************************
* Function Code
******************************************************************************/

/******************************************************************************
* int main(void)
* Main function
* David Tolsma, 05/25/2020
******************************************************************************/
int main(void)
{
	SystemClock_Init();

	Tim_Timer2Init();
	Gpio_Init();

	xTaskCreate( 	testTask,        	/* Function that implements the task. */
                    "testTask",         /* Text name for the task. */
                    100,      			/* Stack size in words, not bytes. */
                    ( void * ) 0,    	/* Parameter passed into the task. */
                    1,					/* Priority at which the task is created. */
                    &testTaskHandle	   	/* Used to pass out the created task's handle. */
	);

	vTaskStartScheduler();



	while(1){} // We should never reach here, error trap.
}
/*****************************************************************************/

/******************************************************************************
* void testTask(void)
* test task
* David Tolsma, 05/25/2020
******************************************************************************/
void testTask(void * pvParameters)
{
    // Enable NVIC interrupt
    NVIC_EnableIRQ(TIM2_IRQn);

	while(1){

		ulTaskNotifyTake( pdTRUE,          /* Clear the notification value before
                                           exiting. */
                          portMAX_DELAY ); /* Block indefinitely. */

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
	}
}
/*****************************************************************************/


void gpioOn(void){
	Gpio_SetPin(PP_1_PORT, PP_1_PIN);
	Gpio_SetPin(PP_2_PORT, PP_2_PIN);
	Gpio_SetPin(PP_3_PORT, PP_3_PIN);
	Gpio_SetPin(PP_4_PORT, PP_4_PIN);
	Gpio_SetPin(PP_5_PORT, PP_5_PIN);
	Gpio_SetPin(PP_6_PORT, PP_6_PIN);
	Gpio_SetPin(PP_7_PORT, PP_7_PIN);
	Gpio_SetPin(PP_8_PORT, PP_8_PIN);

	Gpio_SetPin(LSD_1_PORT, LSD_1_PIN);
	Gpio_SetPin(LSD_2_PORT, LSD_2_PIN);
	Gpio_SetPin(LSD_3_PORT, LSD_3_PIN);
	Gpio_SetPin(LSD_4_PORT, LSD_4_PIN);
	Gpio_SetPin(LSD_5_PORT, LSD_5_PIN);
	Gpio_SetPin(LSD_6_PORT, LSD_6_PIN);
	Gpio_SetPin(LSD_7_PORT, LSD_7_PIN);
	Gpio_SetPin(LSD_8_PORT, LSD_8_PIN);

	Gpio_SetPin(DEV_LED_PORT, DEV_LED_PIN);

	puts("Turning all outputs on. \n");

}


void gpioOff(void){
	Gpio_ResetPin(PP_1_PORT, PP_1_PIN);
	Gpio_ResetPin(PP_2_PORT, PP_2_PIN);
	Gpio_ResetPin(PP_3_PORT, PP_3_PIN);
	Gpio_ResetPin(PP_4_PORT, PP_4_PIN);
	Gpio_ResetPin(PP_5_PORT, PP_5_PIN);
	Gpio_ResetPin(PP_6_PORT, PP_6_PIN);
	Gpio_ResetPin(PP_7_PORT, PP_7_PIN);
	Gpio_ResetPin(PP_8_PORT, PP_8_PIN);

	Gpio_ResetPin(LSD_1_PORT, LSD_1_PIN);
	Gpio_ResetPin(LSD_2_PORT, LSD_2_PIN);
	Gpio_ResetPin(LSD_3_PORT, LSD_3_PIN);
	Gpio_ResetPin(LSD_4_PORT, LSD_4_PIN);
	Gpio_ResetPin(LSD_5_PORT, LSD_5_PIN);
	Gpio_ResetPin(LSD_6_PORT, LSD_6_PIN);
	Gpio_ResetPin(LSD_7_PORT, LSD_7_PIN);
	Gpio_ResetPin(LSD_8_PORT, LSD_8_PIN);

	Gpio_ResetPin(DEV_LED_PORT, DEV_LED_PIN);

	printf("Turning all outputs off. \n");

}


void TIM2_IRQHandler(void){
	CLEAR_BIT(TIM2->SR, TIM_SR_UIF);

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	vTaskNotifyGiveFromISR(testTaskHandle, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
