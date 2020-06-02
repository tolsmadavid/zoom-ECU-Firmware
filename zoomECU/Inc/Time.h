/******************************************************************************
* File:                    Time.h
* Author:                  David Tolsma
* Date Modified:           05/25/2020
* Breif Description:       Initialization and control of system timers
******************************************************************************/
#ifndef TIME_H
#define TIME_H

/******************************************************************************
* Includes
******************************************************************************/


/******************************************************************************
* Defines
******************************************************************************/


/******************************************************************************
* Public Function Prototypes
******************************************************************************/
    /******************************************************************************
    * void TimeTimer2Init(void)
    * Sets up timer 2 to act as a global microsecond timer.
    * David Tolsma, 05/25/2020
    ******************************************************************************/
    void Time_Timer2Init(void);

    /******************************************************************************
    * int32_t Time_GetTimeuSeconds(void)
    * Returns the current value for microseconds
    * David Tolsma, 05/25/2020
    ******************************************************************************/
    uint32_t Time_GetTimeuSeconds(void);

/******************************************************************************
* Public Variables
******************************************************************************/


#endif // ifdef TIME_H
