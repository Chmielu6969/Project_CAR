/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTN_USER_Pin GPIO_PIN_13
#define BTN_USER_GPIO_Port GPIOC
#define LCD_D4_Pin GPIO_PIN_0
#define LCD_D4_GPIO_Port GPIOC
#define LCD_D5_Pin GPIO_PIN_1
#define LCD_D5_GPIO_Port GPIOC
#define LCD_D6_Pin GPIO_PIN_2
#define LCD_D6_GPIO_Port GPIOC
#define LCD_D7_Pin GPIO_PIN_3
#define LCD_D7_GPIO_Port GPIOC
#define JOY_VRX_Pin GPIO_PIN_0
#define JOY_VRX_GPIO_Port GPIOA
#define JOY_VRY_Pin GPIO_PIN_1
#define JOY_VRY_GPIO_Port GPIOA
#define JOY_SW_Pin GPIO_PIN_2
#define JOY_SW_GPIO_Port GPIOA
#define M2_AIN2_Pin GPIO_PIN_4
#define M2_AIN2_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define M2_BIN1_Pin GPIO_PIN_4
#define M2_BIN1_GPIO_Port GPIOC
#define M2_BIN2_Pin GPIO_PIN_5
#define M2_BIN2_GPIO_Port GPIOC
#define M1_STDBY_Pin GPIO_PIN_0
#define M1_STDBY_GPIO_Port GPIOB
#define M1_AIN1_Pin GPIO_PIN_1
#define M1_AIN1_GPIO_Port GPIOB
#define M2_PWMB_Pin GPIO_PIN_7
#define M2_PWMB_GPIO_Port GPIOC
#define M2_STDBY_Pin GPIO_PIN_8
#define M2_STDBY_GPIO_Port GPIOC
#define M2_AIN1_Pin GPIO_PIN_9
#define M2_AIN1_GPIO_Port GPIOC
#define M2_PWMA_Pin GPIO_PIN_8
#define M2_PWMA_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_10
#define LCD_RS_GPIO_Port GPIOC
#define LCD_E_Pin GPIO_PIN_12
#define LCD_E_GPIO_Port GPIOC
#define M1_AIN2_Pin GPIO_PIN_3
#define M1_AIN2_GPIO_Port GPIOB
#define M1_PWMA_Pin GPIO_PIN_4
#define M1_PWMA_GPIO_Port GPIOB
#define M1_BIN1_Pin GPIO_PIN_5
#define M1_BIN1_GPIO_Port GPIOB
#define SERVO_PWM_Pin GPIO_PIN_6
#define SERVO_PWM_GPIO_Port GPIOB
#define M1_BIN2_Pin GPIO_PIN_8
#define M1_BIN2_GPIO_Port GPIOB
#define M1_PWMB_Pin GPIO_PIN_9
#define M1_PWMB_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
