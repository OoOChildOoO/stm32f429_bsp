/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_sdram.h"
#include "bsp_lcd.h"
#include "bsp_spi_flash.h"
#include "bsp_ut600d.h"
#include "bsp_beep.h"
#include "bsp_led.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define BSP_TEST 1
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
#define LCD_BG_EN_Pin GPIO_PIN_11
#define LCD_BG_EN_GPIO_Port GPIOI
#define LCD_BG_PWM_Pin GPIO_PIN_9
#define LCD_BG_PWM_GPIO_Port GPIOF
#define BEEP_Pin GPIO_PIN_1
#define BEEP_GPIO_Port GPIOA
#define WCS1_Pin GPIO_PIN_4
#define WCS1_GPIO_Port GPIOH
#define WCS4_Pin GPIO_PIN_5
#define WCS4_GPIO_Port GPIOC
#define WCS2_Pin GPIO_PIN_0
#define WCS2_GPIO_Port GPIOB
#define UT600D_DATA_Pin GPIO_PIN_12
#define UT600D_DATA_GPIO_Port GPIOB
#define LED_RUN_Pin GPIO_PIN_8
#define LED_RUN_GPIO_Port GPIOC
#define LED_FALUT_Pin GPIO_PIN_9
#define LED_FALUT_GPIO_Port GPIOC
#define USB_OTG_FS_EN_Pin GPIO_PIN_8
#define USB_OTG_FS_EN_GPIO_Port GPIOA
#define WCS3_Pin GPIO_PIN_15
#define WCS3_GPIO_Port GPIOA
#define LED_USB_Pin GPIO_PIN_10
#define LED_USB_GPIO_Port GPIOC
#define LED_SD_Pin GPIO_PIN_11
#define LED_SD_GPIO_Port GPIOC
#define UT600D_RST_Pin GPIO_PIN_3
#define UT600D_RST_GPIO_Port GPIOB
#define UT600D_CLK_Pin GPIO_PIN_4
#define UT600D_CLK_GPIO_Port GPIOB
#define UT600D_BUSY_Pin GPIO_PIN_5
#define UT600D_BUSY_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
