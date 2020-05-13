/**
 ******************************************************************************
 * File:                    GPIO.h
 * Author:                  David Tolsma
 * Breif Description:       Initialization and Control for GPIO pins
 ******************************************************************************
 * Long Description:
 *
 * Provides low-level initialization and configuration for GPIO pins for the 
 * zoomECU board. This module utalizes the STM32Cube Low-Level Drivers.
 *
 ******************************************************************************
 */


#ifndef GPIO_H
#define GPIO_H


//Definitions for GPIO Pins
#define Low_Side_Driver_1_Pin (0x1UL << 9) 
#define Low_Side_Driver_2_Pin (0x1UL << 8) 
#define Low_Side_Driver_3_Pin (0x1UL << 7) 
#define Low_Side_Driver_4_Pin (0x1UL << 6)

#define Low_Side_Driver_5_Pin (0x1UL << 15) 
#define Low_Side_Driver_6_Pin (0x1UL << 14) 
#define Low_Side_Driver_7_Pin (0x1UL << 13) 
#define Low_Side_Driver_8_Pin (0x1UL << 12)  


#define Low_Side_Driver_1_Port GPIOC
#define Low_Side_Driver_2_Port GPIOC
#define Low_Side_Driver_3_Port GPIOC
#define Low_Side_Driver_4_Port GPIOC

#define Low_Side_Driver_5_Port GPIOD
#define Low_Side_Driver_6_Port GPIOD
#define Low_Side_Driver_7_Port GPIOD
#define Low_Side_Driver_8_Port GPIOD



#endif //GPIO_H
