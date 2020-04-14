/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"
#include "stm32g4xx_ll_pwr.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PP5_Pin GPIO_PIN_2
#define PP5_GPIO_Port GPIOE
#define PP6_Pin GPIO_PIN_3
#define PP6_GPIO_Port GPIOE
#define PP7_Pin GPIO_PIN_4
#define PP7_GPIO_Port GPIOE
#define PP8_Pin GPIO_PIN_5
#define PP8_GPIO_Port GPIOE
#define ST_EN_Pin GPIO_PIN_2
#define ST_EN_GPIO_Port GPIOC
#define ST_DIR_Pin GPIO_PIN_3
#define ST_DIR_GPIO_Port GPIOC
#define ST_STEP_Pin GPIO_PIN_2
#define ST_STEP_GPIO_Port GPIOF
#define SD_DETECT_Pin GPIO_PIN_4
#define SD_DETECT_GPIO_Port GPIOC
#define LSD9_Pin GPIO_PIN_12
#define LSD9_GPIO_Port GPIOD
#define LSD7_Pin GPIO_PIN_13
#define LSD7_GPIO_Port GPIOD
#define LSD6_Pin GPIO_PIN_14
#define LSD6_GPIO_Port GPIOD
#define LSD5_Pin GPIO_PIN_15
#define LSD5_GPIO_Port GPIOD
#define LSD4_Pin GPIO_PIN_6
#define LSD4_GPIO_Port GPIOC
#define LSD3_Pin GPIO_PIN_7
#define LSD3_GPIO_Port GPIOC
#define LSD2_Pin GPIO_PIN_8
#define LSD2_GPIO_Port GPIOC
#define LSD1_Pin GPIO_PIN_9
#define LSD1_GPIO_Port GPIOC
#define PP1_Pin GPIO_PIN_4
#define PP1_GPIO_Port GPIOB
#define PP2_Pin GPIO_PIN_5
#define PP2_GPIO_Port GPIOB
#define PP3_Pin GPIO_PIN_6
#define PP3_GPIO_Port GPIOB
#define PP4_Pin GPIO_PIN_7
#define PP4_GPIO_Port GPIOB
#define NEUTRAL_SW_Pin GPIO_PIN_0
#define NEUTRAL_SW_GPIO_Port GPIOE
#define IDLE_SW_Pin GPIO_PIN_1
#define IDLE_SW_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
