/******************************************************************************
* File:                    TriggerDecoder.h
* Author:                  David Tolsma
* Date Modified:           05/25/2020
* Breif Description:       Brief Description of Module
******************************************************************************/
#ifndef TRIGGERDECODER_H
#define TRIGGERDECODER_H

/******************************************************************************
* Includes
******************************************************************************/
#include "FreeRTOS.h"
#include "event_groups.h"


/******************************************************************************
* Defines
******************************************************************************/


/******************************************************************************
* Public Function Prototypes
******************************************************************************/
    /******************************************************************************
    * void TriggerDecoder_Init(void)
    * Initialize the trigger module
    * David Tolsma, 05/25/2020
    ******************************************************************************/
    void TriggerDecoder_Init(void);

    /******************************************************************************
    * void TriggerDecoder_Task(void)
    * Pends on an event, then processes the event and updates the trigger
    * status structure.
    * David Tolsma, 05/25/2020
    ******************************************************************************/
    void TriggerDecoder_Task(void * pvParameters);

    /******************************************************************************
    * float TriggerDecoder_GetRPM(void)
    * Returns the current rpm estimate
    * David Tolsma, 05/25/2020
    ******************************************************************************/
    float TriggerDecoder_GetRPM(void);

    /******************************************************************************
    * float TriggerDecoder_GetCurrentAngle(void)
    * Returns the current engine angle estimate
    * David Tolsma, 05/25/2020
    ******************************************************************************/
    float TriggerDecoder_GetCurrentAngle(void);

    /******************************************************************************
    * float TriggerDecoder_GetuSPerDegree(void)
    * Returns the number of microseconds needed to traverse one degree
    * David Tolsma, 05/25/2020
    ******************************************************************************/
    float TriggerDecoder_GetUsPerDegree(void);
    
    /******************************************************************************
    * float TriggerDecoder_GetDegreePerUs(void)
    * Returns the number of microseconds needed to traverse one degree
    * David Tolsma, 05/25/2020
    ******************************************************************************/
    float TriggerDecoder_GetDegreePerUs(void);

    /******************************************************************************
    * uint32_t TriggerDecoder_GetSyncStatus(void)
    * Determines if trigger decoder is synced
    * David Tolsma, 05/25/2020
    ******************************************************************************/
    int32_t TriggerDecoder_GetSyncStatus(void);

    int32_t TriggerDecoder_IsCranking(void);

    int32_t TriggerDecoder_GetSyncConfidece(void);

/******************************************************************************
* Public Variables
******************************************************************************/

#endif // ifdef TRIGGERDECODER_H
