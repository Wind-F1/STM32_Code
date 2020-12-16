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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32l4xx_hal.h"

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
#define SD_EN_Pin GPIO_PIN_1
#define SD_EN_GPIO_Port GPIOF
#define A1_Pin GPIO_PIN_2
#define A1_GPIO_Port GPIOG
#define A0_Pin GPIO_PIN_3
#define A0_GPIO_Port GPIOG
#define EN_Pin GPIO_PIN_4
#define EN_GPIO_Port GPIOG
#define ADC_CLK_Pin GPIO_PIN_0
#define ADC_CLK_GPIO_Port GPIOD
#define MTI_DRDY_Pin GPIO_PIN_5
#define MTI_DRDY_GPIO_Port GPIOD
#define MTI_DRDY_EXTI_IRQn EXTI9_5_IRQn
#define MTI_Reset_Pin GPIO_PIN_6
#define MTI_Reset_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */
#define huartPC huart1 
#define  hadcMCM hadc1 
#define   hadcTDM  hadc3

#define hi2cMTI hi2c1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
