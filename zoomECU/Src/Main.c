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
#include "Time.h"
#include "IgnitionControl.h"
#include "TriggerDecoder.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
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
void myTimerCallback(TimerHandle_t myTimerHandle);

/******************************************************************************
* Private Variables (static)
******************************************************************************/
TaskHandle_t TestTaskHandle = NULL;
TimerHandle_t myTimerHandle = NULL;

uint32_t angle;

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

	Time_Timer2Init();

	Gpio_Init();

	TriggerDecoder_Init();

	myTimerHandle =  xTimerCreate
                 	( "myTimer",
                  	1,
                   	pdTRUE,
                   	(void *) 0,
                   	myTimerCallback );

	xTimerStart(myTimerHandle, 0);

    //xTaskCreate( 	TestTask,        	    /* Function that implements the task. */
	//				"testTask",       /* Text name for the task. */
	//				100,      			        /* Stack size in words, not bytes. */
	//				( void * ) 0,    	        /* Parameter passed into the task. */
	//				1,					        /* Priority at which the task is created. */
	//				&TestTaskHandle);	/* Used to pass out the created task's handle. */


	vTaskStartScheduler();

	while(1){} // We should never reach here, error trap.
}
/*****************************************************************************/

void myTimerCallback(TimerHandle_t myTimerHandle)
{
	angle = ((uint32_t) TriggerDecoder_GetCurrentAngle());
	angle = ((uint32_t) TriggerDecoder_GetCurrentAngle());
	angle = ((uint32_t) TriggerDecoder_GetCurrentAngle());
	angle = ((uint32_t) TriggerDecoder_GetCurrentAngle());
	angle = ((uint32_t) TriggerDecoder_GetCurrentAngle());
	angle = ((uint32_t) TriggerDecoder_GetCurrentAngle());
	angle = ((uint32_t) TriggerDecoder_GetCurrentAngle());
	angle = ((uint32_t) TriggerDecoder_GetCurrentAngle());
	angle = ((uint32_t) TriggerDecoder_GetCurrentAngle());
	angle = ((uint32_t) TriggerDecoder_GetCurrentAngle());
}


void TestTask(void * pvParameters){


	while(1){

	}
}
