/**
 ******************************************************************************
 * File:                    timers.c
 * Author:                  David Tolsma
 * Breif Description:       Initialization and Control for timers
 ******************************************************************************
 * Long Description:
 *
 * Initialization and control for timers.
 *
 ******************************************************************************
 */

#include "stm32g4xx.h"
#include "stm32g474xx.h"

// Setup of uSecond timer
void timersTimer2Init(void){

    // Enable clock to timer
    SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM2EN);

    // Enable timer 2
    SET_BIT(TIM2->CR1, TIM_CR1_CEN);

    // Set prescaler, timer frequency = input clock / prescaler +1
    // For a 170 Mhz core, and a 1 Mhz timer, we need a prescaler value
    // of 170 - 1 = 169.
    WRITE_REG(TIM2->PSC, 169);

    // Set auto-reload register (when the timer rolls back to zero)
    // Resets once per microsecond
    WRITE_REG(TIM2->ARR, 1000000);

    // Enable interupts from timer
    WRITE_REG(TIM2->DIER, TIM_DIER_UIE);

    // Enable trigger events from timer
    WRITE_REG(TIM2->EGR, TIM_EGR_TG);

    // Enable NVIC interrupt
    NVIC_EnableIRQ(TIM2_IRQn);
}

