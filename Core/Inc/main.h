/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

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
#define key3_Pin GPIO_PIN_1
#define key3_GPIO_Port GPIOC
#define key3_EXTI_IRQn EXTI1_IRQn
#define key2_Pin GPIO_PIN_2
#define key2_GPIO_Port GPIOC
#define key2_EXTI_IRQn EXTI2_IRQn
#define key1_Pin GPIO_PIN_3
#define key1_GPIO_Port GPIOC
#define key1_EXTI_IRQn EXTI3_IRQn
#define led3_Pin GPIO_PIN_4
#define led3_GPIO_Port GPIOA
#define led2_Pin GPIO_PIN_5
#define led2_GPIO_Port GPIOA
#define led_Pin GPIO_PIN_6
#define led_GPIO_Port GPIOA
#define GX_SCL_Pin GPIO_PIN_10
#define GX_SCL_GPIO_Port GPIOB
#define GX_SDA_Pin GPIO_PIN_11
#define GX_SDA_GPIO_Port GPIOB
#define DC_AN2_Pin GPIO_PIN_14
#define DC_AN2_GPIO_Port GPIOB
#define DC_AN1_Pin GPIO_PIN_15
#define DC_AN1_GPIO_Port GPIOB
#define OLED_CLK_Pin GPIO_PIN_3
#define OLED_CLK_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_4
#define OLED_DC_GPIO_Port GPIOB
#define OLED_DATA_Pin GPIO_PIN_5
#define OLED_DATA_GPIO_Port GPIOB
#define OLED_RES_Pin GPIO_PIN_8
#define OLED_RES_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
