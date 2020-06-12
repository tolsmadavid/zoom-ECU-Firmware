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
#include "EngineController.h"

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


/******************************************************************************
* Private Variables (static)
******************************************************************************/


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

	IgnitionControl_Init();

	EngineController_Init();

	TriggerDecoder_Init();

	vTaskStartScheduler();

	while(1){} // We should never reach here, error trap.
}
/*****************************************************************************/

