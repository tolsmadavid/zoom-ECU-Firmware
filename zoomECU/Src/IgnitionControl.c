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

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"

/******************************************************************************
* Defines
******************************************************************************/
#define IGN_SCH_1  (0x1UL << 0)
#define IGN_SCH_2  (0x1UL << 1)
#define IGN_SCH_3  (0x1UL << 2)
#define IGN_SCH_4  (0x1UL << 3)

/******************************************************************************
* Public Variables
******************************************************************************/


/******************************************************************************
* Private Function Prototypes (static)
******************************************************************************/
void IgnitionControl_EventCreationTask(void * pvParameters);

void testEndCallback(void);
void testStartCallback(void);

float IgnitionControl_calcNextIgnitionAngle(void);
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

TaskHandle_t IgnitionControlEventCreationTaskHandle = NULL;

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
    ignitionSchedule[0].startCallback = &testStartCallback;
    ignitionSchedule[0].endCallback = &testEndCallback;
    
    ignitionSchedule[1].startCallback = &testStartCallback;
    ignitionSchedule[1].endCallback = &testEndCallback;

    ignitionSchedule[2].startCallback = &testStartCallback;
    ignitionSchedule[2].endCallback = &testEndCallback;

    ignitionSchedule[3].startCallback = &testStartCallback;
    ignitionSchedule[3].endCallback = &testEndCallback;


    // Create the trigger decoder task
    xTaskCreate(IgnitionControl_EventCreationTask,              /* Function that implements the task. */
                "ignitionEventCreationTask",                    /* Text name for the task. */
                100,      			                            /* Stack size in words, not bytes. */
                ( void * ) 0,    	                            /* Parameter passed into the task. */
                1,					                            /* Priority at which the task is created. */
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

    float currentAngle;
    uint32_t currentTime;
    float nextIgnAngle;
    int32_t nextDwellTime;
    float deltaAngle;
    

    while(1){

        xTaskNotifyWait(0,                  //do not clear any bits on entry
                        bitsToClearOnExit,
                        &notificationValue,
                        portMAX_DELAY);

        bitsToClearOnExit = 0;
        
        if(notificationValue & IGN_SCH_1){
            SET_BIT(bitsToClearOnExit, IGN_SCH_1);

            nextIgnAngle = IgnitionControl_calcNextIgnitionAngle();
            currentAngle = TriggerDecoder_GetCurrentAngle();
            currentTime = Time_GetTimeuSeconds();

            if(nextIgnAngle > currentAngle){
                deltaAngle = nextIgnAngle - currentAngle;
            }
            else{
                deltaAngle = (720 - currentAngle) + nextIgnAngle;
            }

            ignitionSchedule[0].endTime = currentTime + (TriggerDecoder_GetUsPerDegree() * deltaAngle);
            ignitionSchedule[0].startTime = ignitionSchedule[0].endTime - IgnitionControl_calcDwellTime();

        }

        if(notificationValue & IGN_SCH_2){
            SET_BIT(bitsToClearOnExit, IGN_SCH_2);
            //repeat above
        }
        if(notificationValue & IGN_SCH_3){
            SET_BIT(bitsToClearOnExit, IGN_SCH_3);
            //repeat above
        }
        if(notificationValue & IGN_SCH_4){
            SET_BIT(bitsToClearOnExit, IGN_SCH_4);
            //repeat above
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
    }

    else if (irqStatus & TIM_SR_CC2IF){
        CLEAR_BIT(TIM2->SR, TIM_SR_CC2IF);
        x = 1;
        notificationBit = IGN_SCH_2;
    }

    else if (irqStatus & TIM_SR_CC3IF){
        CLEAR_BIT(TIM2->SR, TIM_SR_CC3IF);
        x = 2;
        notificationBit = IGN_SCH_3;
    }
    
    else if (irqStatus & TIM_SR_CC4IF){
        CLEAR_BIT(TIM2->SR, TIM_SR_CC4IF);
        x = 3;
        notificationBit = IGN_SCH_4;
    }

    // Check for update interupts
    else if (irqStatus & TIM_SR_UIF){
        CLEAR_BIT(TIM2->SR, TIM_SR_UIF);
        x = -1; // in this case, we dont want to run the state machine
    }

    if (x != -1){
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

                // Inform the scheduler task that we need a new schedule
                xTaskNotifyFromISR( IgnitionControlEventCreationTaskHandle,
                                    notificationBit,
                                    eSetBits,
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
    else{while(1);} // Error trap, we should never get here if not interupt bits are set
    
    // If we have woken a higer priority task, we should yield to that task
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
/*****************************************************************************/

// These are test functions to act as placeholders for the real ignition GPIO calls.
void testStartCallback(void){
	gpioOn();
}

void testEndCallback(void){
	gpioOff();
}

float IgnitionControl_calcNextIgnitionAngle(void){
    return ((float) 10); // returns a fixed angle of 10 degrees
}

int32_t IgnitionControl_calcDwellTime(void){
    return 10000; // returns a fixed dwell time of 10mS
}
