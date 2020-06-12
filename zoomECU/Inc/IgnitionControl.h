/******************************************************************************
* File:                    IgnitionControl.h
* Author:                  David Tolsma
* Date Modified:           05/25/2020
* Breif Description:       Brief Description of Module
******************************************************************************/
#ifndef IGNITIONCONTROL_H
#define IGNITIONCONTROL_H

/******************************************************************************
* Includes
******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"

/******************************************************************************
* Defines
******************************************************************************/
#define IGN_SCH_1  (0x1UL << 0)
#define IGN_SCH_2  (0x1UL << 1)
#define IGN_SCH_3  (0x1UL << 2)
#define IGN_SCH_4  (0x1UL << 3)

/******************************************************************************
* Public Function Prototypes
******************************************************************************/
    
    /******************************************************************************
    * void IgnitionControl_Init(void)
    * Creates and schedules all ignition events
    * David Tolsma, 05/25/2020
    ******************************************************************************/
    void IgnitionControl_Init(void);
    /*****************************************************************************/
    
/******************************************************************************
* Public Variables
******************************************************************************/
// This is the handle to the event creation task.
TaskHandle_t IgnitionControlEventCreationTaskHandle;

// This event group is set when one of the 4 ignition schedules is cleared.
EventGroupHandle_t ignitionScheduleFinishedEventGroup;

#endif // ifdef IGNITIONCONTROL_H
