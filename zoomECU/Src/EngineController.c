/******************************************************************************
* File:                    EngineControler.c
* Author:                  David Tolsma
* Date Modified:           05/25/2020
* Breif Description:       Brief Description of Module
*******************************************************************************
* Includes
******************************************************************************/
#include "TriggerDecoder.h"
#include "IgnitionControl.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"

#include <stdio.h>

/******************************************************************************
* Defines
******************************************************************************/


/******************************************************************************
* Public Variables
******************************************************************************/


/******************************************************************************
* Private Variables (static)
******************************************************************************/
TaskHandle_t EngineControllerTaskHandle;

typedef enum{
    OFF,
    CRANKING,
    RUNNING
} engineState_t;


/******************************************************************************
* Private Function Prototypes (static)
******************************************************************************/
void EngineController_task(void * pvParameters);
engineState_t EngineController_Off(void);
engineState_t EngineController_Cranking(void);
engineState_t EngineController_Running(void);


/******************************************************************************
* Function Code
******************************************************************************/


/******************************************************************************
* void EngineController_Init(void)
* This task initialises the engine controller module.
* David Tolsma, 05/25/2020
******************************************************************************/
void EngineController_Init(void){


    // Create the trigger decoder task
    xTaskCreate(EngineController_task,              /* Function that implements the task. */
                "engineControllerTask",                    /* Text name for the task. */
                200,      			                            /* Stack size in words, not bytes. */
                ( void * ) 0,    	                            /* Parameter passed into the task. */
                1,					                            /* Priority at which the task is created. */
                &EngineControllerTaskHandle);	    /* Used to pass out the created task's handle. */
}


/*****************************************************************************/


/******************************************************************************
* void EngineController_task(void)
* This task manages all engine functions.
* David Tolsma, 05/25/2020
******************************************************************************/
void EngineController_task(void * pvParameters){

    // This entire engine controller task implements a state machine indicating
    // the state of the engine. The states can be OFF, CRANKING, RUNNING.

    // When we are in the OFF state, we repeatedly check to see if the engine is
    // cranking. If if it is, we change states to CRANKING.

    // In CRANKING, we schedule ignition events using a fixed ignition angle and
    // check RPM unill we reach a RUNNING threashold in which case we change to 
    // RUNNING state. If the RPMs drop below a threshold, we transition to OFF.

    // In RUNNING, we check schedule ignition events not on a fixed ignition 
    // angle. We check to see if the RPMs drop below a threshold in which case
    // we transition to off. 

    // This task is implemented as a never ending loop. We do not need the idle 
    // task as we do not need to delete any tasks in the program and therefore
    // do not need it to clean up any memory.

    engineState_t engineState = OFF;

    while(1){

        switch(engineState){
            case OFF:{
                engineState = EngineController_Off();
                break;
            }
            case CRANKING:{
                engineState = EngineController_Cranking();
                break;
            }
            case RUNNING:{
                engineState = EngineController_Running();
                break;
            }
            default: {while(1);}
        }

    }
}
/*****************************************************************************/


/******************************************************************************
* engineState_t EngineController_Off(void)
* This handles OFF state logic.
* David Tolsma, 05/25/2020
******************************************************************************/
engineState_t EngineController_Off(void){
    // This ensures that the state does not change unless intentially changed below
    engineState_t nextState = OFF;

    if(TriggerDecoder_IsCranking()) {nextState = CRANKING;}
    
    return nextState;
}
/*****************************************************************************/



/******************************************************************************
* engineState_t EngineController_Cranking(void)
* This handles CRANKING state logic.
* David Tolsma, 05/25/2020
******************************************************************************/
engineState_t EngineController_Cranking(void){
    // This ensures that the state does not change unless intentially changed below
    engineState_t nextState = CRANKING;
    int32_t tempRPM;
    int32_t ignScheduleFinished;

    // Since no ignition schedules have been set, we cannot pend of the ignition schedule
    // finished event group. We need to manually trigger the first schedule when transitioning
    // from OFF state to CRANKING state. 

    xTaskNotify(IgnitionControlEventCreationTaskHandle,
                IGN_SCH_1 | IGN_SCH_2 | IGN_SCH_3 | IGN_SCH_4,
                eSetBits);

    tempRPM = TriggerDecoder_GetRPM();

    while(tempRPM > 50 & tempRPM < 500){

    	ignScheduleFinished = xEventGroupWaitBits(
			ignitionScheduleFinishedEventGroup,
			IGN_SCH_1 | IGN_SCH_2 | IGN_SCH_3 | IGN_SCH_4,  // Bits to wait for
			pdTRUE,     									// Clear bits on exit
			pdFALSE,    									// Delay on all bits
			portMAX_DELAY);

        xTaskNotify(IgnitionControlEventCreationTaskHandle,
                    (ignScheduleFinished | 0x1111),
                    eSetBits);

        tempRPM = TriggerDecoder_GetRPM();
    }

    // Check what state to change to.
    if(tempRPM > 500) {nextState = RUNNING;}
    else if (tempRPM < 50) {nextState = OFF;}

    return nextState;
}
/*****************************************************************************/



/******************************************************************************
* engineState_t EngineController_Running(void)
* This handles RUNNING state logic.
* David Tolsma, 05/25/2020
******************************************************************************/
engineState_t EngineController_Running(void){
    // This ensures that the state does not change unless intentially changed below
    engineState_t nextState = RUNNING;
    static int32_t tempRPM;
    int32_t ignScheduleFinished;

    tempRPM = TriggerDecoder_GetRPM();

    while(tempRPM > 50){

    	ignScheduleFinished = xEventGroupWaitBits(
			ignitionScheduleFinishedEventGroup,
			IGN_SCH_1 | IGN_SCH_2 | IGN_SCH_3 | IGN_SCH_4,  // Bits to wait for
			pdTRUE,     									// Clear bits on exit
			pdFALSE,    									// Delay on all bits
			portMAX_DELAY);

        xTaskNotify(IgnitionControlEventCreationTaskHandle,
                    (ignScheduleFinished | 0x1111),
                    eSetBits);

        tempRPM = TriggerDecoder_GetRPM();

    }
    

    if(tempRPM < 50) {nextState = OFF;}

    return nextState;
}
/*****************************************************************************/