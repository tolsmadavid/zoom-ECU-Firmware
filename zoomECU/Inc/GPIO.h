/**
 ******************************************************************************
 * File:                    GPIO.h
 * Author:                  David Tolsma
 * Breif Description:       Initialization and Control for GPIO PINs
 ******************************************************************************
 * Long Description:
 *
 * Provides low-level initialization and configuration for GPIO PINs for the 
 * zoomECU board. This module utalizes the STM32Cube Low-Level Drivers.
 *
 ******************************************************************************
 */


#ifndef GPIO_H
#define GPIO_H

#include "stm32g474xx.h"

void GpioInit(void);
uint32_t GPIO_ReadInputPort(GPIO_TypeDef *GPIOx);
uint32_t GPIO_ReadInputPin(GPIO_TypeDef *GPIOx, uint32_t Pin);
void GPIO_SetPin(GPIO_TypeDef *GPIOx, uint32_t Pin);
void GPIO_ResetPin(GPIO_TypeDef *GPIOx, uint32_t Pin);
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t Pin);

#endif //GPIO_H
