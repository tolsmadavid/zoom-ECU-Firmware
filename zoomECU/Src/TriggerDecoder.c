/******************************************************************************
* File:                    TriggerDecoder.c
* Author:                  David Tolsma
* Date Modified:           05/25/2020
* Breif Description:       Brief Description of Module
*******************************************************************************
* Includes
******************************************************************************/
#include "TriggerDecoder.h"
#include "PinoutConfiguration.h"
#include "Gpio.h"
#include "Time.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"

#include "stm32g4xx.h"

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
typedef enum{
    PRIMARY_RISE,
    PRIMARY_FALL,
    SECONDARY_RISE,
    SECONDARY_FALL,
}triggerEventID_t;

typedef enum{
    PRIMARY_HIGH,
    PRIMARY_LOW,
    SECONDARY_HIGH,
    SECONDARY_LOW
}triggerValue_t;

struct triggerEvent_t{
    uint32_t timeStamp;
    triggerEventID_t eventID;
    triggerValue_t primaryTriggerValue;
    triggerValue_t secondaryTriggerValue;
};

struct triggerStatus_t{
    uint32_t hasSync;
    uint32_t pastPrimaryEvents[4];
    uint32_t pastSecondaryEvents[4];
    uint32_t lastPrimaryEventNumber;
    uint32_t lastSecondaryEventNumber;
    float primaryEventAngles[8];
    float secondaryEventAngles[4];
};

struct triggerStatus_t triggerStatus = {
    .hasSync = 0,
    .primaryEventAngles = {105, 175, 285, 355, 465, 535, 635, 715}
};


QueueHandle_t triggerEventQHandle;

/******************************************************************************
* Function Code
******************************************************************************/


/******************************************************************************
* void TriggerDecoder_Init(void)
* Initialize the trigger module
* David Tolsma, 05/25/2020
******************************************************************************/
void TriggerDecoder_Init(void){
    // Enable the System config controller
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);

    // Set up EXTI1 to trigger on CAM GPIO Pin
    MODIFY_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI1, SYSCFG_EXTICR1_EXTI1_PB);
    // Set up EXTI3 to trigger on CRANK GPIO Pin
    MODIFY_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI3, SYSCFG_EXTICR1_EXTI3_PA);

    // Clear EXTI1 intterupt mask
    SET_BIT(EXTI->IMR1, EXTI_IMR1_IM1);
    // Clear EXTI3 ntterupt mask
    SET_BIT(EXTI->IMR1, EXTI_IMR1_IM3);

    // Enable EXTI1 rising edge triggers 
    SET_BIT(EXTI->RTSR1, EXTI_RTSR1_RT1);
    // Enable EXTI3 rising edge triggers 
    SET_BIT(EXTI->RTSR1, EXTI_RTSR1_RT3);

    // Enable EXTI1 falling edge triggers 
    SET_BIT(EXTI->FTSR1, EXTI_FTSR1_FT1);
    // Enable EXTI3 falling edge triggers 
    SET_BIT(EXTI->FTSR1, EXTI_FTSR1_FT3);

    // Set NVIC Priotities to allow FreeRTOS calls in interupt
    NVIC_SetPriority(EXTI1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(EXTI3_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);


}
/*****************************************************************************/

/******************************************************************************
* void TriggerDecoder_Init(void)
* Initialize the trigger module
* David Tolsma, 05/25/2020
******************************************************************************/
void TriggerDecoderTask(void * pvParameters){
    triggerEventQHandle = xQueueCreate( 10, sizeof( struct triggerEvent_t) );
    struct triggerEvent_t eventBeingProcessed;

    // Enable interupt
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);

	while(1){
        
        // Pend on an event coming from the ISRs
        xQueueReceive(triggerEventQHandle,
					  &eventBeingProcessed,
					  portMAX_DELAY);

        if((eventBeingProcessed.eventID == PRIMARY_RISE) || (eventBeingProcessed.eventID == PRIMARY_FALL)){
            // Increment event number (and set to zero on overflow)
            if( triggerStatus.lastPrimaryEventNumber < 7){
                triggerStatus.lastPrimaryEventNumber++;
            }
            else if (triggerStatus.lastPrimaryEventNumber == 7){
                triggerStatus.lastPrimaryEventNumber = 0;
                gpioOn();
            }
            else{
                while(1); // Error trap, should never get here.
            }

            if(triggerStatus.lastPrimaryEventNumber == 1){
            	gpioOff();
            }

            // Shift log, and add new event to log of past events (implemented without a for loop for speed)
            triggerStatus.pastPrimaryEvents[3] = triggerStatus.pastPrimaryEvents[2];
            triggerStatus.pastPrimaryEvents[2] = triggerStatus.pastPrimaryEvents[1];
            triggerStatus.pastPrimaryEvents[1] = triggerStatus.pastPrimaryEvents[0];
            triggerStatus.pastPrimaryEvents[0] = eventBeingProcessed.timeStamp;

            // Check to see if the last added event makes sense with respect to what the secondary trigger is doing

            // If the trigger does not have sync, check if this is an event where we can establish sync
            // On a primary trigger event, we can only establish sync if on a falling edge, we see that the secondary
            // trigger is high. If it is high, then we know that we just saw primary event #4. We will then change
            // the last seen event for the primary trigger to event #3, secondary trigger to event #0 and establish
            // a sync condition
            if(triggerStatus.hasSync == 0){
                if( (eventBeingProcessed.eventID == PRIMARY_FALL) && (eventBeingProcessed.secondaryTriggerValue == SECONDARY_HIGH) ){
                    triggerStatus.lastPrimaryEventNumber = 3;
                    triggerStatus.lastSecondaryEventNumber = 0;
                    triggerStatus.hasSync = 1;
                }
            }
            // If the trigger has sync, check if this is an event that matches the expected secondary trigger value, if
            // it does not, then we have lost sync and should set .hasSync = 0.
            else if(triggerStatus.hasSync == 1){
                switch(triggerStatus.lastPrimaryEventNumber){
                    case 0: if(eventBeingProcessed.secondaryTriggerValue != SECONDARY_LOW) {triggerStatus.hasSync = 0;}
                        break;
                    case 1: if(eventBeingProcessed.secondaryTriggerValue != SECONDARY_LOW) {triggerStatus.hasSync = 0;}
                        break;
                    case 2: if(eventBeingProcessed.secondaryTriggerValue != SECONDARY_HIGH) {triggerStatus.hasSync = 0;}
                        break;
                    case 3: if(eventBeingProcessed.secondaryTriggerValue != SECONDARY_HIGH) {triggerStatus.hasSync = 0;}
                        break;
                    case 4: if(eventBeingProcessed.secondaryTriggerValue != SECONDARY_LOW) {triggerStatus.hasSync = 0;}
                        break;
                    case 5: if(eventBeingProcessed.secondaryTriggerValue != SECONDARY_LOW) {triggerStatus.hasSync = 0;}
                        break;
                    case 6: if(eventBeingProcessed.secondaryTriggerValue != SECONDARY_HIGH) {triggerStatus.hasSync = 0;}
                        break;
                    case 7: if(eventBeingProcessed.secondaryTriggerValue != SECONDARY_LOW) {triggerStatus.hasSync = 0;}
                        break;
                    default: while(1); // Error trap, we should never get here.
                    	break;
                }
            }
        } 

        else if((eventBeingProcessed.eventID == SECONDARY_RISE) || (eventBeingProcessed.eventID == SECONDARY_FALL)){
            
            // Increment event number (and set to zero on overflow)
            if( triggerStatus.lastSecondaryEventNumber < 3){
                triggerStatus.lastSecondaryEventNumber++;
            }
            else if (triggerStatus.lastSecondaryEventNumber == 3){
                triggerStatus.lastSecondaryEventNumber = 0;
            }
            else{
                while(1); // Error trap, should never get here.
            }

            // Shift log, and add new event to log of past events (imSecondaryed without a for loop for speed)
            triggerStatus.pastSecondaryEvents[3] = triggerStatus.pastSecondaryEvents[2];
            triggerStatus.pastSecondaryEvents[2] = triggerStatus.pastSecondaryEvents[1];
            triggerStatus.pastSecondaryEvents[1] = triggerStatus.pastSecondaryEvents[0];
            triggerStatus.pastSecondaryEvents[0] = eventBeingProcessed.timeStamp;

            // Check to see if the last added event makes sense with respect to what the secondary trigger is doing

            // If the trigger does not have sync, check if this is an event where we can establish sync
            // On a secondary trigger event we can only establish sync on a falling edge. When secondary is falling
            // and the primary trigger is low, then we determine that .lastPrimaryEventNumber = 3, and
            // .lastSecondaryEventNumber = 1. When the secondary is falling and the primary is high, then we determing
            // that .lastPrimaryEventNumber = 6 and .lastSecondaryEventNumber = 3.
            if(triggerStatus.hasSync == 0){
                if(eventBeingProcessed.eventID == SECONDARY_FALL){
                    if(eventBeingProcessed.primaryTriggerValue == PRIMARY_LOW){
                        triggerStatus.lastPrimaryEventNumber = 3;
                        triggerStatus.lastSecondaryEventNumber = 1;
                        triggerStatus.hasSync = 1;
                    }
                    else if(eventBeingProcessed.primaryTriggerValue == PRIMARY_HIGH){
                        triggerStatus.lastPrimaryEventNumber = 6;
                        triggerStatus.lastSecondaryEventNumber = 3;
                        triggerStatus.hasSync = 1;
                    }
                    else{
                        while(1); // Error trap, should never get here.
                    }
                }
            }

            else if(triggerStatus.hasSync == 1){
                switch(triggerStatus.lastSecondaryEventNumber){
                    case 0: if(eventBeingProcessed.primaryTriggerValue != PRIMARY_LOW) {triggerStatus.hasSync = 0;}
                        break;
                    case 1: if(eventBeingProcessed.primaryTriggerValue != PRIMARY_LOW) {triggerStatus.hasSync = 0;}
                        break;
                    case 2: if(eventBeingProcessed.primaryTriggerValue != PRIMARY_LOW) {triggerStatus.hasSync = 0;}
                        break;
                    case 3: if(eventBeingProcessed.primaryTriggerValue != PRIMARY_HIGH) {triggerStatus.hasSync = 0;}
                        break;
                    default: while(1); // Error trap, should never get here.
                        break;
                }
            }

            else{
                while(1); // Error trap, should never get here.
            }
        }

        else{
            while(1); // we should never get here
        }
	}
}
/*****************************************************************************/




// CAM is the secondary trigger
void EXTI1_IRQHandler(void){

    BaseType_t xHigherPriorityTaskWoken;
    struct triggerEvent_t triggerEvent;

    // Get timestamp as soon as possible for best accuracy
    triggerEvent.timeStamp = Time_GetTimeuSeconds();

    // We are in EXTI1_IRQHandler beacuse of a CAM sensor event, however we do not
    // know if it is a rising or falling edge. We check here for that info.
    if(Gpio_ReadInputPin(CAM_PORT, CAM_PIN)){
        triggerEvent.eventID = SECONDARY_RISE;
        triggerEvent.secondaryTriggerValue = SECONDARY_HIGH;
    }
    else{
        triggerEvent.eventID = SECONDARY_FALL;
        triggerEvent.secondaryTriggerValue = SECONDARY_LOW;
    }

    // Log what the other trigger value currently is
    if(Gpio_ReadInputPin(CRANK_PORT, CRANK_PIN)){
        triggerEvent.primaryTriggerValue = PRIMARY_HIGH;
    }
    else{
        triggerEvent.primaryTriggerValue = PRIMARY_LOW;
    }

    // Clear interupt source
    SET_BIT(EXTI->PR1, EXTI_PR1_PIF1);
    
    // We have not yet woken a higher priority task
    xHigherPriorityTaskWoken = pdFALSE;

    // Post the trigerEvent struct to the queue
    xQueueSendFromISR( triggerEventQHandle, (void *) &triggerEvent, &xHigherPriorityTaskWoken );

    // If we have woken a higer priority task, we should yield to that task
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// CRANK is the primary trigger
void EXTI3_IRQHandler(void){
    BaseType_t xHigherPriorityTaskWoken;
    struct triggerEvent_t triggerEvent;

    // Get timestamp as soon as possible for best accuracy
    triggerEvent.timeStamp = Time_GetTimeuSeconds();

    // We are in EXTI3_IRQHandler beacuse of a CRANK sensor event, however we do not
    // know if it is a rising or falling edge. We check here for that info.
    if(Gpio_ReadInputPin(CRANK_PORT, CRANK_PIN)){
        triggerEvent.eventID = PRIMARY_RISE;
        triggerEvent.primaryTriggerValue = PRIMARY_HIGH;
    }
    else{
        triggerEvent.eventID = PRIMARY_FALL;
        triggerEvent.primaryTriggerValue = PRIMARY_LOW;
    }

    // Log what the other trigger value currently is
    if(Gpio_ReadInputPin(CAM_PORT, CAM_PIN)){
        triggerEvent.secondaryTriggerValue = SECONDARY_HIGH;
    }
    else{
        triggerEvent.secondaryTriggerValue = SECONDARY_LOW;
    }

    // Clear interupt source
    SET_BIT(EXTI->PR1, EXTI_PR1_PIF3);
    
    // We have not yet woken a higher priority task
    xHigherPriorityTaskWoken = pdFALSE;

    // Post the trigerEvent struct to the queue
    xQueueSendFromISR( triggerEventQHandle, (void *) &triggerEvent, &xHigherPriorityTaskWoken );

    // If we have woken a higer priority task, we should yield to that task
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
