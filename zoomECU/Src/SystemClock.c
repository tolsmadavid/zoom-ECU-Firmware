/******************************************************************************
 * File:                    SystemClock.c
 * Author:                  David Tolsma
 * Date Modified:           05/25/2020
 * Breif Description:       Initialization for system core clocks
 *****************************************************************************/





#include "stm32g4xx.h"
#include "SystemClock.h"




#define SYSTEM_CORE_CLOCK 170000000

// Deviates from standard style conventions for compatibility with existing libraries
uint32_t SystemCoreClock = SYSTEM_CORE_CLOCK;





/**
******************************************************************************
*   File:                    SystemClock.c
*   Function:                void SystemClock_Init(void)
*   Author:                  David Tolsma
*   Breif Description:       Initialization for system core clocks
******************************************************************************
**/

void SystemClock_Init(void){

// Set up all external oscilators

    // Enable clock to PWR module
    SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_PWREN);

    // For core clocks above 80 Mhz, the main voltage regulator must be set
    // to boost mode. This is done by clearing the PWR_CR5 register, bit 8.
    CLEAR_BIT(PWR->CR5, PWR_CR5_R1MODE);

    // Increase the number of wait states for the flash controller for higher speeds
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_4WS);

    // Enable HSE oscilator
    SET_BIT(RCC->CR, RCC_CR_HSEON);

    // Wait till HSE is stable and ready
    while(READ_BIT(RCC->CR, RCC_CR_HSERDY) == 0){}

    // In order to modify BDCR we must unlock it
    SET_BIT(PWR->CR1, PWR_CR1_DBP);

    // Optionaly lower LSE oscilator drive to lower battery consumption
    // MODIFY_REG(RCC->BDCR, RCC_BDCR_LSEDRV, 0);

    // Enable LSE oscilator
    SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);

    // Wait till LSE is stable and ready
    while(READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY) == 0){}

    // Configure PLL settings
    // Set PLL source to HSE oscilator
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC, RCC_PLLCFGR_PLLSRC_HSE);

    // Set PLL M to a division of 12
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, (RCC_PLLCFGR_PLLM_3 | RCC_PLLCFGR_PLLM_1 | RCC_PLLCFGR_PLLM_0));

    // Set PLL N to a multiplication of 85
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, (85 << RCC_PLLCFGR_PLLN_Pos));

    // Set PLL R to a division of 2
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLR, 0);

    // Enable PLL Output
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLREN);

    // Enable PLL
    SET_BIT(RCC->CR, RCC_CR_PLLON);

    // Wait for PLL to be stable and ready
    while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) == 0){}

    // Set AHB Prescaler to a division of 1
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, 0);

    // Set AHB1 Prescaler to a division of 1
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, 0);

    // Set AHB2 Prescaler to a division of 1
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, 0);

    // Set PLL to be the core clock source
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);

    // Wait untill system clock is ready (ensuring that the hardware reports PLL as clock source)
    while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    // Enable use access to the FPU
    SCB->CPACR |= ((3UL << (10*2))|(3UL << (11*2)));  /* set CP10 and CP11 Full Access */

    // Set up SysTick timer for 1ms interupts
    SysTick_Config(SystemCoreClock/1000);
}  




