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
static void testTask(void * pvParameters);
void myTimerCallback(TimerHandle_t myTimerHandle);
void testEndCallback(void);
void testStartCallback(void);

/******************************************************************************
* Private Variables (static)
******************************************************************************/
TaskHandle_t testTaskHandle = NULL;
TimerHandle_t myTimerHandle = NULL;
TaskHandle_t TriggerDecoderTaskHandle = NULL;

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

	Time_Timer2Init();
	Gpio_Init();
	TriggerDecoder_Init();

	uint32_t test;
	test = Gpio_ReadInputPin(CRANK_PORT, CRANK_PIN);
	if(test){test = 69;}

	//testSchedule.startCallback = &testStartCallback;
	//testSchedule.endCallback = &testEndCallback;

	//testSchedule.startTime = 100000;
	//testSchedule.endTime = 200000;
	
	//testSchedule.status = PENDING;

	xTaskCreate( 	testTask,        	/* Function that implements the task. */
                    "testTask",         /* Text name for the task. */
                    100,      			/* Stack size in words, not bytes. */
                    ( void * ) 0,    	/* Parameter passed into the task. */
                    1,					/* Priority at which the task is created. */
                    &testTaskHandle	   	/* Used to pass out the created task's handle. */
	);

	xTaskCreate( 	TriggerDecoderTask,        	/* Function that implements the task. */
	                    "triggerDecoderTask",   /* Text name for the task. */
	                    100,      			/* Stack size in words, not bytes. */
	                    ( void * ) 0,    	/* Parameter passed into the task. */
	                    1,					/* Priority at which the task is created. */
	                    &TriggerDecoderTaskHandle	/* Used to pass out the created task's handle. */
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

	myTimerHandle =  xTimerCreate
                 	( "myTimer",
                  	1000,
                   	pdTRUE,
                   	(void *) 0,
                   	myTimerCallback );

	xTimerStart(myTimerHandle, 0);

	while(1){
		ulTaskNotifyTake( pdTRUE,          /* Clear the notification value before
                                           exiting. */
                          portMAX_DELAY ); /* Block indefinitely. */

	}
}
/*****************************************************************************/

void myTimerCallback(TimerHandle_t myTimerHandle)
{
	xTaskNotifyGive(testTaskHandle);
}


void testStartCallback(void){
	gpioOn();
}

void testEndCallback(void){
	gpioOff();
}

