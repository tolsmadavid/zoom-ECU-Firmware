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

    // Enable timer 2
    SET_BIT(TIM2->CR1, TIM_CR1_CEN);

    // Set prescaler, timer frequency = input clock / prescaler +1
    // For a 170 Mhz core, and a 1 Mhz timer, we need a prescaler value
    // of 170 - 1 = 169.
    WRITE_REG(TIM2->PSC, 169);

    // Set auto-reload register (when the timer rolls back to zero)
    // Resets once per second
    WRITE_REG(TIM2->ARR, (0xFFFFFFFF));

    // Enable interupts from timer update event
    //WRITE_REG(TIM2->DIER, TIM_DIER_UIE);

    // Enable interupts from timer compare registers 1-4
    //WRITE_REG(TIM2->DIER, TIM_DIER_CC1IE);
    //WRITE_REG(TIM2->DIER, TIM_DIER_CC2IE);
    //WRITE_REG(TIM2->DIER, TIM_DIER_CC3IE);
    //WRITE_REG(TIM2->DIER, TIM_DIER_CC4IE);

    // Clear all interupts
    CLEAR_REG(TIM2->SR);

    // Set interupt priority to allow for FreeRTOS system calls
    NVIC_SetPriority(TIM2_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
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

/******************************************************************************
* void TIM2_IRQHandler(void)
* Handler for timer 2, increments an upper 32 bit integer to count the number 
* of overflows of timer 2. Timer 2 overflows approximatly every 71 minutes.
* David Tolsma, 05/25/2020
******************************************************************************/
void TIM2_IRQHandler(void){
    uint32_t irqStatus;
    irqStatus = TIM2->SR;

    //Check for count compare interupts
    if(irqStatus & TIM_SR_CC1IF){ 
        CLEAR_BIT(TIM2->SR, TIM_SR_CC1IF);
        /*switch(testSchedule.status){
            case PENDING:{
                testSchedule.startCallback();
                testSchedule.status = RUNNING;
                TIM2->CCR1 = testSchedule.endTime;
                break;
            }
            case RUNNING:{
                testSchedule.endCallback();
                testSchedule.status = PENDING;
                TIM2->CCR1 = testSchedule.startTime;
                break;
            }
            case OFF:{
                while(1); //should never be in IRQ with no schedule
                break;
            }
        } */
    }
    if (irqStatus & TIM_SR_CC2IF){
        CLEAR_BIT(TIM2->SR, TIM_SR_CC2IF);

    }
    if (irqStatus & TIM_SR_CC3IF){
        CLEAR_BIT(TIM2->SR, TIM_SR_CC3IF);

    }
    if (irqStatus & TIM_SR_CC4IF){
        CLEAR_BIT(TIM2->SR, TIM_SR_CC4IF);

    }
    // Check for update interupts
    if (irqStatus & TIM_SR_UIF){
        CLEAR_BIT(TIM2->SR, TIM_SR_UIF);
    }
}
/*****************************************************************************/
