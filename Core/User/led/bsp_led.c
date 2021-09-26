/*
 * @Author: your name
 * @Date: 2021-09-17 15:09:56
 * @LastEditTime: 2021-09-25 15:02:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \simple_7inches_v2\Core\User\led\bsp_led.c
 */

#include "bsp_led.h"

volatile LED_BreathTypdef BreathLedList[] = {
    {.Cnt = 0, .Dir = 1, .Enable = 1, .CntMax = 1000, .CntMin = 100, .timHandle = &LED_PWM_Handler, .channel = LED_RUN_TIM_CHANNEL}};
/**
 * @description: 初始化开关型LED
 * @param {*}
 * @return {*}
 */
void LED_GPIO_Config(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  LED_FALUT_CLK_ENABLE();
  LED_USB_CLK_ENABLE();
  LED_SD_CLK_ENABLE();

  LED_FALUT_OFF();
  LED_USB_OFF();
  LED_SD_OFF();

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pin = LED_FALUT_PIN;
  HAL_GPIO_Init(LED_FALUT_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LED_USB_PIN;
  HAL_GPIO_Init(LED_USB_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LED_SD_PIN;
  HAL_GPIO_Init(LED_SD_GPIO_PORT, &GPIO_InitStruct);
}

/**
 * @description: 初始化呼吸灯型LED
 * @param {*}
 * @return {*}
 */
void LED_PWM_Config(void)
{
  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  LED_RUN_CLK_ENABLE();
  /**TIM8 GPIO Configuration
    PC8     ------> TIM8_CH3
    */
  GPIO_InitStruct.Pin = LED_RUN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = LED_RUN_AF;
  HAL_GPIO_Init(LED_RUN_GPIO_PORT, &GPIO_InitStruct);

  LED_PWM_Handler.Instance = TIM8;
  LED_PWM_Handler.Init.Prescaler = 84 - 1;
  LED_PWM_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
  LED_PWM_Handler.Init.Period = 1000;
  LED_PWM_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  LED_PWM_Handler.Init.RepetitionCounter = 0;
  LED_PWM_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&LED_PWM_Handler) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&LED_PWM_Handler, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&LED_PWM_Handler) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&LED_PWM_Handler, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&LED_PWM_Handler, &sConfigOC, LED_RUN_TIM_CHANNEL) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&LED_PWM_Handler, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */
  HAL_TIM_Base_Start(&LED_PWM_Handler);
  HAL_TIM_PWM_Start(&LED_PWM_Handler, LED_RUN_TIM_CHANNEL);
  /* USER CODE END TIM8_Init 2 */

  LED_PWM_Enable(&BreathLedList[0]);
}

/**
 * @description: 开始指定的呼吸灯
 * @param {LED_BreathTypdef} BreathLed
 * @return {*}
 */
void LED_PWM_Enable(volatile LED_BreathTypdef *BreathLed)
{
  BreathLed->Enable = 1;
}

/**
 * @description: 停止指定的呼吸灯
 * @param {LED_BreathTypdef} BreathLed
 * @return {*}
 */
void LED_PWM_Disable(volatile LED_BreathTypdef *BreathLed)
{
  BreathLed->Enable = 0;
}

/**
 * @description: 处理呼吸灯函数，存放在tim = 1ms的回调函数中
 * @param {*}
 * @return {*}
 */
void LED_PWM_DealTaskList(void)
{
  for (uint8_t i = 0; i < sizeof(BreathLedList) / sizeof(LED_BreathTypdef); i++)
  {
    if (BreathLedList[i].Enable)
    {
      if (BreathLedList[i].Dir)
      {
        BreathLedList[i].Cnt++;
        if (BreathLedList[i].Cnt >= BreathLedList[i].CntMax)
        {
          BreathLedList[i].Dir = 0;
        }
      }
      else
      {
        BreathLedList[i].Cnt--;
        if (BreathLedList[i].Cnt <= BreathLedList[i].CntMin)
        {
          BreathLedList[i].Dir = 1;
        }
      }
      __HAL_TIM_SET_COMPARE(BreathLedList[i].timHandle, BreathLedList[i].channel, BreathLedList[i].Cnt);
    }
    else
    {
      BreathLedList[i].Cnt = 0;
      __HAL_TIM_SET_COMPARE(BreathLedList[i].timHandle, BreathLedList[i].channel, BreathLedList[i].Cnt);
    }
  }
}

/*********************************************END OF FILE**********************/
