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

#include "IgnitionControl.h"
#include "TriggerDecoder.h"
#include "Time.h"
#include "Gpio.h"
#include "PinoutConfiguration.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"

#include <stdio.h>

/******************************************************************************
* Defines
******************************************************************************/


/******************************************************************************
* Public Variables
******************************************************************************/
float currentAngleGlobal;


/******************************************************************************
* Private Function Prototypes (static)
******************************************************************************/
void IgnitionControl_EventCreationTask(void * pvParameters);

void testEndCallback1(void);
void testStartCallback1(void);
void testEndCallback2(void);
void testStartCallback2(void);
void testEndCallback3(void);
void testStartCallback3(void);
void testEndCallback4(void);
void testStartCallback4(void);

float IgnitionControl_calcNextIgnitionAngle(int32_t ignSchedule);
int32_t IgnitionControl_calcDwellTime(void);

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

struct Schedule ignitionSchedule[4];

/******************************************************************************
* Function Code
******************************************************************************/

// Lets get a queue where we can post an event to schedule, and it will
// handle everything from there. We can then make a basic test task that 
// calls a dummy "timing scheduling" function with a fixed timing.

/******************************************************************************
* void IgnitionControl_Init(void)
* Creates and schedules all ignition events
* David Tolsma, 05/25/2020
******************************************************************************/
void IgnitionControl_Init(void){

    // Set up callbacks for ignition schedules
    ignitionSchedule[0].startCallback = &testStartCallback1;
    ignitionSchedule[0].endCallback = &testEndCallback1;
    
    ignitionSchedule[1].startCallback = &testStartCallback2;
    ignitionSchedule[1].endCallback = &testEndCallback2;

    ignitionSchedule[2].startCallback = &testStartCallback3;
    ignitionSchedule[2].endCallback = &testEndCallback3;

    ignitionSchedule[3].startCallback = &testStartCallback4;
    ignitionSchedule[3].endCallback = &testEndCallback4;

    // Create ignition schedule event group
    ignitionScheduleFinishedEventGroup = xEventGroupCreate();

    // Create the trigger decoder task
    xTaskCreate(IgnitionControl_EventCreationTask,              /* Function that implements the task. */
                "ignitionEventCreationTask",                    /* Text name for the task. */
                400,      			                            /* Stack size in words, not bytes. */
                ( void * ) 0,    	                            /* Parameter passed into the task. */
                3,					                            /* Priority at which the task is created. */
                &IgnitionControlEventCreationTaskHandle);	    /* Used to pass out the created task's handle. */
}
/*****************************************************************************/

/******************************************************************************
* void IgnitionControl_EventCreationTask(void)
* Creates and schedules all ignition events
* David Tolsma, 05/25/2020
******************************************************************************/
void IgnitionControl_EventCreationTask(void * pvParameters){

    uint32_t bitsToClearOnExit;
    uint32_t notificationValue;
    uint32_t status;
    uint32_t endTime;
    uint32_t startTime;

    float currentAngle;
    uint32_t currentTime;
    float nextIgnAngle;
    int32_t dwellTime;
    float deltaAngle;
    int32_t uSPerDegree;

    while(1){

        xTaskNotifyWait(0,                  //do not clear any bits on entry
                        0xffffffff,
                        &notificationValue,
                        portMAX_DELAY);
        
        if(notificationValue & IGN_SCH_1){

            nextIgnAngle = IgnitionControl_calcNextIgnitionAngle(IGN_SCH_1);
            currentAngle = TriggerDecoder_GetCurrentAngle();
            currentTime = Time_GetTimeuSeconds();
            uSPerDegree = TriggerDecoder_GetUsPerDegree();
            dwellTime = IgnitionControl_calcDwellTime();

            if((nextIgnAngle == -1) | (currentAngle == -1) | (uSPerDegree == -1)){
                printf("Error: Ignition 1 not set, failed confidence check. \n");
                xEventGroupSetBits( ignitionScheduleFinishedEventGroup,        /* The event group being updated. */
                                    IGN_SCH_1);                                /* The bits being set. */
            }
            
            else{
                if(nextIgnAngle > currentAngle){
                deltaAngle = nextIgnAngle - currentAngle;
                }
                else{
                    deltaAngle = (720 - currentAngle) + nextIgnAngle;
                }

                endTime = currentTime + (uSPerDegree * deltaAngle);
                ignitionSchedule[0].endTime = endTime;

                startTime = ignitionSchedule[0].endTime - dwellTime;
                ignitionSchedule[0].startTime = startTime;

                if(startTime <= currentTime){
					printf("Error: Ignition 1 not set, event set in past. \n");
					xEventGroupSetBits( ignitionScheduleFinishedEventGroup,        /* The event group being updated. */
										IGN_SCH_1);                                /* The bits being set. */
				}
                else{
                    WRITE_REG(TIM2->CCR1, ignitionSchedule[0].startTime);
                    ignitionSchedule[0].status = PENDING;
                }
            }
        }

        if(notificationValue & IGN_SCH_2){
            nextIgnAngle = IgnitionControl_calcNextIgnitionAngle(IGN_SCH_2);
            currentAngle = TriggerDecoder_GetCurrentAngle();
            currentTime = Time_GetTimeuSeconds();
            uSPerDegree = TriggerDecoder_GetUsPerDegree();
            dwellTime = IgnitionControl_calcDwellTime();

            if((nextIgnAngle == -1) | (currentAngle == -1) | (uSPerDegree == -1)){
                printf("Error: Ignition 2 not set, failed confidence check. \n");
                xEventGroupSetBits( ignitionScheduleFinishedEventGroup,        /* The event group being updated. */
                                    IGN_SCH_2);                                /* The bits being set. */
            }
            
            else{
                if(nextIgnAngle > currentAngle){
                deltaAngle = nextIgnAngle - currentAngle;
                }
                else{
                    deltaAngle = (720 - currentAngle) + nextIgnAngle;
                }

                endTime = currentTime + (uSPerDegree * deltaAngle);
                ignitionSchedule[1].endTime = endTime;

                startTime = ignitionSchedule[1].endTime - dwellTime;
                ignitionSchedule[1].startTime = startTime;

                if(startTime <= currentTime){
					printf("Error: Ignition 2 not set, event set in past. \n");
					xEventGroupSetBits( ignitionScheduleFinishedEventGroup,        /* The event group being updated. */
										IGN_SCH_2);                                /* The bits being set. */
				}
                else{
                    WRITE_REG(TIM2->CCR2, ignitionSchedule[1].startTime);
                    ignitionSchedule[1].status = PENDING;
                }
            }
        }

        if(notificationValue & IGN_SCH_3){
            nextIgnAngle = IgnitionControl_calcNextIgnitionAngle(IGN_SCH_3);
            currentAngle = TriggerDecoder_GetCurrentAngle();
            currentTime = Time_GetTimeuSeconds();
            uSPerDegree = TriggerDecoder_GetUsPerDegree();
            dwellTime = IgnitionControl_calcDwellTime();

            if((nextIgnAngle == -1) | (currentAngle == -1) | (uSPerDegree == -1)){
                printf("Error: Ignition 3 not set, failed confidence check. \n");
                xEventGroupSetBits( ignitionScheduleFinishedEventGroup,        /* The event group being updated. */
                                    IGN_SCH_3);                                /* The bits being set. */
            }
            
            else{
                if(nextIgnAngle > currentAngle){
                deltaAngle = nextIgnAngle - currentAngle;
                }
                else{
                    deltaAngle = (720 - currentAngle) + nextIgnAngle;
                }

                endTime = currentTime + (uSPerDegree * deltaAngle);
                ignitionSchedule[2].endTime = endTime;

                startTime = ignitionSchedule[2].endTime - dwellTime;
                ignitionSchedule[2].startTime = startTime;

                if(startTime <= currentTime){
					printf("Error: Ignition 3 not set, event set in past. \n");
					xEventGroupSetBits( ignitionScheduleFinishedEventGroup,        /* The event group being updated. */
										IGN_SCH_3);                                /* The bits being set. */
				}
                else{
                    WRITE_REG(TIM2->CCR3, ignitionSchedule[2].startTime);
                    ignitionSchedule[2].status = PENDING;
                }
            }
        }

        if(notificationValue & IGN_SCH_4){
            nextIgnAngle = IgnitionControl_calcNextIgnitionAngle(IGN_SCH_4);
            currentAngle = TriggerDecoder_GetCurrentAngle();
            currentTime = Time_GetTimeuSeconds();
            uSPerDegree = TriggerDecoder_GetUsPerDegree();
            dwellTime = IgnitionControl_calcDwellTime();

            if((nextIgnAngle == -1) | (currentAngle == -1) | (uSPerDegree == -1)){
                printf("Error: Ignition 4 not set, failed confidence check. \n");
                xEventGroupSetBits( ignitionScheduleFinishedEventGroup,        /* The event group being updated. */
                                    IGN_SCH_4);                                /* The bits being set. */
            }
            
            else{
                if(nextIgnAngle > currentAngle){
                deltaAngle = nextIgnAngle - currentAngle;
                }
                else{
                    deltaAngle = (720 - currentAngle) + nextIgnAngle;
                }

                endTime = currentTime + (uSPerDegree * deltaAngle);
                ignitionSchedule[3].endTime = endTime;

                startTime = ignitionSchedule[3].endTime - dwellTime;
                ignitionSchedule[3].startTime = startTime;

                if(startTime <= currentTime){
					printf("Error: Ignition 4 not set, event set in past. \n");
					xEventGroupSetBits( ignitionScheduleFinishedEventGroup,        /* The event group being updated. */
										IGN_SCH_4);                                /* The bits being set. */
				}
                else{
                    WRITE_REG(TIM2->CCR4, ignitionSchedule[3].startTime);
                    ignitionSchedule[3].status = PENDING;
                }
            }
        }   
    }
}

/*****************************************************************************/

/******************************************************************************
* void TIM2_IRQHandler(void)
* Handler for timer 2. Timer 2 overflows approximatly every 71 minutes. 
* This interupt handles all ignition schedule callback functions. 
* David Tolsma, 05/25/2020
******************************************************************************/
void TIM2_IRQHandler(void){
    uint32_t irqStatus;
    int32_t x;
    uint32_t notificationBit;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    irqStatus = TIM2->SR;

    //Check for count compare interupts
    if(irqStatus & TIM_SR_CC1IF){
        CLEAR_BIT(TIM2->SR, TIM_SR_CC1IF);
        x = 0;
        notificationBit = IGN_SCH_1;

        switch(ignitionSchedule[x].status){
			case PENDING:{
				ignitionSchedule[x].startCallback();
				ignitionSchedule[x].status = RUNNING;
				TIM2->CCR1 = ignitionSchedule[x].endTime;
				break;
			}
			case RUNNING:{
				ignitionSchedule[x].endCallback();
				ignitionSchedule[x].status = OFF;

				// Inform that the relevant ignition schedule is now off.
				xEventGroupSetBitsFromISR(  ignitionScheduleFinishedEventGroup,      /* The event group being updated. */
											notificationBit,                    /* The bits being set. */
											&xHigherPriorityTaskWoken );

				break;
			}
			case OFF:{
				while(1); //should never be in IRQ with no schedule
				break;
			}
			default:{
				while(1); //should never be in IRQ with no schedule
				break;
			}
		}
    }

    if (irqStatus & TIM_SR_CC2IF){
        CLEAR_BIT(TIM2->SR, TIM_SR_CC2IF);
        x = 1;
        notificationBit = IGN_SCH_2;

        switch(ignitionSchedule[x].status){
			case PENDING:{
				ignitionSchedule[x].startCallback();
				ignitionSchedule[x].status = RUNNING;
				TIM2->CCR2 = ignitionSchedule[x].endTime;
				break;
			}
			case RUNNING:{
				ignitionSchedule[x].endCallback();
				ignitionSchedule[x].status = OFF;

				// Inform that the relevant ignition schedule is now off.
				xEventGroupSetBitsFromISR(  ignitionScheduleFinishedEventGroup,      /* The event group being updated. */
											notificationBit,                    /* The bits being set. */
											&xHigherPriorityTaskWoken );

				break;
			}
			case OFF:{
				while(1); //should never be in IRQ with no schedule
				break;
			}
			default:{
				while(1); //should never be in IRQ with no schedule
				break;
			}
        }
    }

    if (irqStatus & TIM_SR_CC3IF){
        CLEAR_BIT(TIM2->SR, TIM_SR_CC3IF);
        x = 2;
        notificationBit = IGN_SCH_3;

        switch(ignitionSchedule[x].status){
			case PENDING:{
				ignitionSchedule[x].startCallback();
				ignitionSchedule[x].status = RUNNING;
				TIM2->CCR3 = ignitionSchedule[x].endTime;
				break;
			}
			case RUNNING:{
				ignitionSchedule[x].endCallback();
				ignitionSchedule[x].status = OFF;

				// Inform that the relevant ignition schedule is now off.
				xEventGroupSetBitsFromISR(  ignitionScheduleFinishedEventGroup,      /* The event group being updated. */
											notificationBit,                    /* The bits being set. */
											&xHigherPriorityTaskWoken );

				break;
			}
			case OFF:{
				while(1); //should never be in IRQ with no schedule
				break;
			}
			default:{
				while(1); //should never be in IRQ with no schedule
				break;
			}
        }
    }
    
    if (irqStatus & TIM_SR_CC4IF){
        CLEAR_BIT(TIM2->SR, TIM_SR_CC4IF);
        x = 3;
        notificationBit = IGN_SCH_4;

        switch(ignitionSchedule[x].status){
			case PENDING:{
				ignitionSchedule[x].startCallback();
				ignitionSchedule[x].status = RUNNING;
				TIM2->CCR4 = ignitionSchedule[x].endTime;
				break;
			}
			case RUNNING:{
				ignitionSchedule[x].endCallback();
				ignitionSchedule[x].status = OFF;

				// Inform that the relevant ignition schedule is now off.
				xEventGroupSetBitsFromISR(  ignitionScheduleFinishedEventGroup,      /* The event group being updated. */
											notificationBit,                    /* The bits being set. */
											&xHigherPriorityTaskWoken );

				break;
			}
			case OFF:{
				while(1); //should never be in IRQ with no schedule
				break;
			}
			default:{
				while(1); //should never be in IRQ with no schedule
				break;
			}
        }
    }

    // Check for update interupts
    if (irqStatus & TIM_SR_UIF){
        CLEAR_BIT(TIM2->SR, TIM_SR_UIF);
        x = -1; // in this case, we dont want to run the state machine
    }
    
    // If we have woken a higer priority task, we should yield to that task
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
/*****************************************************************************/

// These are test functions to act as placeholders for the real ignition GPIO calls.
void testStartCallback1(void){
	Gpio_SetPin(PP_1_PORT, PP_1_PIN);
}

void testEndCallback1(void){
	Gpio_ResetPin(PP_1_PORT, PP_1_PIN);
}

void testStartCallback2(void){
	Gpio_SetPin(PP_2_PORT, PP_2_PIN);
}

void testEndCallback2(void){
	Gpio_ResetPin(PP_2_PORT, PP_2_PIN);
}

void testStartCallback3(void){
	Gpio_SetPin(PP_3_PORT, PP_3_PIN);
}

void testEndCallback3(void){
	Gpio_ResetPin(PP_3_PORT, PP_3_PIN);
}

void testStartCallback4(void){
	Gpio_SetPin(PP_4_PORT, PP_4_PIN);
}

void testEndCallback4(void){
	Gpio_ResetPin(PP_4_PORT, PP_4_PIN);
}

float IgnitionControl_calcNextIgnitionAngle(int32_t ignSchedule){
    float ignAngle;

    switch(ignSchedule){
        case IGN_SCH_1: {
            ignAngle = 90;
            break;
        }
        case IGN_SCH_2: {
            ignAngle = 270;
            break;
        }
        case IGN_SCH_3: {
            ignAngle = 450;
            break;
        }
        case IGN_SCH_4: {
            ignAngle = 630;
            break;
        }
        default: while(1);
    }
    
    return ignAngle;
}

int32_t IgnitionControl_calcDwellTime(void){
    return 1000; // returns a fixed dwell time of 1mS
}
