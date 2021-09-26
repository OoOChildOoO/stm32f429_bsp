/*
 * @Author: your name
 * @Date: 2021-09-22 15:53:16
 * @LastEditTime: 2021-09-25 15:08:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \simple_7inches_v2\Core\User\ut600d_voice\bsp_ut600d.c
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_BEEP_H__
#define __BSP_BEEP_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#define BEEP_GPIO_ON GPIO_PIN_SET
#define BEEP_GPIO_OFF GPIO_PIN_RESET

#define BEEP_Pin GPIO_PIN_1
#define BEEP_GPIO_Port GPIOA
#define BEEP_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

    typedef struct
    {
        uint32_t cnt;
        uint8_t status;
    } BEEP_TaskListTypdef;
#define BEEP_TASK_NUM 10

#define BEEP_WRITE(x) HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, x)

    void BEEP_Init(void);
    void BEEP_AppendTaskList(uint8_t status, uint32_t cnt);
    void BEEP_ClearTaskList(void);
    void BEEP_DealTaskList(void);
    void BEEP_Single(uint32_t cnt);
    void BEEP_Square(uint8_t times, uint32_t cntON, uint32_t cntOFF);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_UT600D_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
