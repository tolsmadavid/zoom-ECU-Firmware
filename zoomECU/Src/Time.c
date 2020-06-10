/******************************************************************************
* File:                    Time.c
* Author:                  David Tolsma
* Date Modified:           05/25/2020
* Breif Description:       Initialization and Control for timers
*******************************************************************************
* Includes
******************************************************************************/

#include "FreeRTOSConfig.h"
#include "IgnitionControl.h"
#include "Time.h"

#include "stm32g4xx.h"
#include "stm32g474xx.h"

/******************************************************************************
* Defines
******************************************************************************/


/******************************************************************************
* Public Variables
******************************************************************************/


/******************************************************************************
* Private Function Prototypes
******************************************************************************/


/******************************************************************************
* Private Variables
******************************************************************************/
static int32_t secondsSinceBoot;

/******************************************************************************
* Function Code
******************************************************************************/


/******************************************************************************
* void TimeTimer2Init(void)
* Sets up timer 2 to act as a global microsecond timer.
* David Tolsma, 05/25/2020
******************************************************************************/
void Time_Timer2Init(void){

    // Enable clock to timer
    SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM2EN);

    DBGMCU->APB1FZR1 |= DBGMCU_APB1FZR1_DBG_TIM2_STOP; //enable timer 7 stop

    // Set prescaler, timer frequency = input clock / prescaler +1
    // For a 170 Mhz core, and a 1 Mhz timer, we need a prescaler value
    // of 170 - 1 = 169.
    WRITE_REG(TIM2->PSC, 169);

    // Set auto-reload register (when the timer rolls back to zero)
    // Resets once per second
    WRITE_REG(TIM2->ARR, (0xFFFFFFFF));

    // Reset timer to update the prescaler and autoreload register.
    WRITE_REG(TIM2->EGR, TIM_EGR_UG);

    // Enable timer 2
    SET_BIT(TIM2->CR1, TIM_CR1_CEN);

    // Enable interupts from timer update event
    //WRITE_REG(TIM2->DIER, TIM_DIER_UIE);

    // Enable interupts from timer compare registers 1-4
    WRITE_REG(TIM2->DIER, TIM_DIER_CC1IE);
    //WRITE_REG(TIM2->DIER, TIM_DIER_CC2IE);
    //WRITE_REG(TIM2->DIER, TIM_DIER_CC3IE);
    //WRITE_REG(TIM2->DIER, TIM_DIER_CC4IE);

    // Clear all interupts
    CLEAR_REG(TIM2->SR);

    // Set interupt priority to allow for FreeRTOS system calls
    NVIC_SetPriority(TIM2_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);

    // Enalbe NVIC for timer 2
    NVIC_EnableIRQ(TIM2_IRQn);
}
/*****************************************************************************/

/******************************************************************************
* int32_t Time_GetTimeuSeconds(void)
* Returns the current value for microseconds seconds
* David Tolsma, 05/25/2020
******************************************************************************/
uint32_t Time_GetTimeuSeconds(void){
	return TIM2->CNT;
}
/*****************************************************************************/
