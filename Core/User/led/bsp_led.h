/*
 * @Author: your name
 * @Date: 2021-09-22 15:53:16
 * @LastEditTime: 2021-09-25 15:02:34
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \simple_7inches_v2\Core\User\ut600d_voice\bsp_ut600d.c
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "main.h"

#define LED_FALUT_PIN GPIO_PIN_9
#define LED_FALUT_GPIO_PORT GPIOC
#define LED_FALUT_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED_FALUT_TOGGLE() HAL_GPIO_TogglePin(LED_FALUT_GPIO_PORT, LED_FALUT_PIN)
#define LED_FALUT_OFF() HAL_GPIO_WritePin(LED_FALUT_GPIO_PORT, LED_FALUT_PIN, GPIO_PIN_RESET)
#define LED_FALUT_ON() HAL_GPIO_WritePin(LED_FALUT_GPIO_PORT, LED_FALUT_PIN, GPIO_PIN_SET)

#define LED_USB_PIN GPIO_PIN_10
#define LED_USB_GPIO_PORT GPIOC
#define LED_USB_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED_USB_TOGGLE() HAL_GPIO_TogglePin(LED_USB_GPIO_PORT, LED_USB_PIN)
#define LED_USB_OFF() HAL_GPIO_WritePin(LED_USB_GPIO_PORT, LED_USB_PIN, GPIO_PIN_SET)
#define LED_USB_ON() HAL_GPIO_WritePin(LED_USB_GPIO_PORT, LED_USB_PIN, GPIO_PIN_RESET)

#define LED_SD_PIN GPIO_PIN_11
#define LED_SD_GPIO_PORT GPIOC
#define LED_SD_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED_SD_TOGGLE() HAL_GPIO_TogglePin(LED_SD_GPIO_PORT, LED_SD_PIN)
#define LED_SD_OFF() HAL_GPIO_WritePin(LED_SD_GPIO_PORT, LED_SD_PIN, GPIO_PIN_SET)
#define LED_SD_ON() HAL_GPIO_WritePin(LED_SD_GPIO_PORT, LED_SD_PIN, GPIO_PIN_RESET)

	typedef struct
	{
		uint8_t Enable;	 //ʹ��
		uint16_t Dir;	 //�������� 0:���¼��� 1:���ϼ���
		uint32_t Cnt;	 //����ֵ
		uint32_t CntMax; //����ֵ-���
		uint32_t CntMin; //����ֵ-��С
		TIM_HandleTypeDef *timHandle;
		uint8_t channel;
	} LED_BreathTypdef;

	extern TIM_HandleTypeDef htim8;
#define LED_PWM_Handler htim8
#define LED_RUN_TIM_CHANNEL TIM_CHANNEL_3
#define LED_RUN_PIN GPIO_PIN_8
#define LED_RUN_GPIO_PORT GPIOC
#define LED_RUN_CLK_ENABLE()      \
	__HAL_RCC_GPIOC_CLK_ENABLE(); \
	__HAL_RCC_TIM8_CLK_ENABLE()
#define LED_RUN_AF GPIO_AF3_TIM8

	extern volatile LED_BreathTypdef BreathLedList[];

	void LED_GPIO_Config(void);
	void LED_PWM_Config(void);
	void LED_PWM_Enable(volatile LED_BreathTypdef *BreathLed);
	void LED_PWM_Disable(volatile LED_BreathTypdef *BreathLed);
	void LED_PWM_DealTaskList(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_UT600D_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
