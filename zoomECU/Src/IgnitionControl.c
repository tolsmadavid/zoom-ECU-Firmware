/******************************************************************************
* File:                    IgnitionControl.c
* Author:                  David Tolsma
* Date Modified:           05/25/2020
* Breif Description:       Brief Description of Module
*******************************************************************************
* Includes
******************************************************************************/
#include "stm32g4xx.h"
#include "stm32g474xx.h"

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
enum scheduleStatusStates {OFF, PENDING, RUNNING}; //The 3 statuses that a schedule can have

struct Schedule {
  	volatile uint32_t startTime;
  	volatile uint32_t endTime;

	void (*startCallback)(); //Start Callback function for schedule
  	void (*endCallback)(); //Start Callback function for schedule

  	volatile enum scheduleStatusStates status;
};

struct Schedule testSchedule;


/******************************************************************************
* Function Code
******************************************************************************/


/******************************************************************************
* void IgnitionControl_EventCreationTask(void)
* Creates and schedules all ignition events
* David Tolsma, 05/25/2020
******************************************************************************/
/*
void IgnitionControl_EventCreationTask(void){

    while(1){
        // Calculate next pair of ignition events
        IgnitionControl_calcNextIgnitionAngle(angle);
        IgnitionControl_calcDwellTime(timeInUS);
        IgnitionControl_calcTimeToAngle(timeInUS);
        timeToStart = timeToEnd - timeToDwell;

        // Schedule first event
        IgnitionControl_scheduleEvent();
        state = pending;
        pendOnIRQ;

        // Schedule next event
        IgnitionControl_scheduleEvent();
        state = running;
        pendOnIRQ;

        // Ignition events complete
        state = off;
    }
}
*/

/*****************************************************************************/