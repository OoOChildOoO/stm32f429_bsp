/*
 * @Author: your name
 * @Date: 2021-09-22 15:53:16
 * @LastEditTime: 2021-09-26 11:21:28
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \simple_7inches_v2\Core\User\ut600d_voice\bsp_ut600d.c
 */

#include "bsp_beep.h"
#include <string.h>

static volatile uint32_t BeepCnt = 0;   //ms
static volatile uint8_t BeepEnable = 0; //使能beep计数
static BEEP_TaskListTypdef BeepTaskList[BEEP_TASK_NUM];

/**
 * @description: 初始化UT600D接口
 * @param {*}
 * @return {*}
 */
void BEEP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    BEEP_GPIO_CLK_ENABLE();

    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, BEEP_GPIO_OFF);

    /*Configure GPIO pins : PBPin PBPin PBPin */
    GPIO_InitStruct.Pin = BEEP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BEEP_GPIO_Port, &GPIO_InitStruct);
}

/**
 * @description: 添加计数任务
 * @param {uint8_t} status 状态
 * @param {uint32_t} cnt cnt 计数时间ms
 * @return {*}
 */
void BEEP_AppendTaskList(uint8_t status, uint32_t cnt)
{
    uint8_t i = 0;
    for (; i < BEEP_TASK_NUM; i++)
    {
        if (BeepTaskList[i].cnt == 0)
        {
            BeepTaskList[i].status = status;
            if (cnt > 0xFFFF)
                cnt = 0xFFFF;
            BeepTaskList[i].cnt = cnt;
            break;
        }
    }
}

/**
 * @description: 清除计数任务
 * @param {*}
 * @return {*}
 */
void BEEP_ClearTaskList(void)
{
    BEEP_WRITE(BEEP_GPIO_OFF);
    BeepCnt = 0;
    memset(BeepTaskList, 0, sizeof(BeepTaskList));
}

/**
 * @description: 处理计数任务
 * @param {*}
 * @return {*}
 */
void BEEP_DealTaskList(void)
{
    if (BeepEnable)
    {
        if (BeepCnt != 0)
        {
            BeepCnt--;
        }
        else
        {
            if (BeepTaskList[0].cnt == 0)
            {
                BEEP_WRITE(BEEP_GPIO_OFF);
                return;
            }
            BeepCnt = BeepTaskList[0].cnt;
            BEEP_WRITE(BeepTaskList[0].status);
            /*数组前移*/
            for (uint8_t i = 0; i < BEEP_TASK_NUM - 1; i++)
            {
                memcpy(&BeepTaskList[i], &BeepTaskList[i + 1], sizeof(BEEP_TaskListTypdef));
            }
            memset(&BeepTaskList[BEEP_TASK_NUM - 1], 0, sizeof(BEEP_TaskListTypdef));
        }
    }
}

/**
 * @description: 一声(单脉冲),会清除正在播放的任务
 * @param {uint32_t} cnt 计数时间ms
 * @return {*}
 */
void BEEP_Single(uint32_t cnt)
{
    BeepEnable = 0;
    BEEP_ClearTaskList();
    BEEP_AppendTaskList(BEEP_GPIO_ON, cnt);
    BeepEnable = 1;
}

/**
 * @description: 多声(多脉冲),会清除正在播放的任务
 * @param {uint8_t} times 次数
 * @param {uint32_t} cntON 开计数时间ms
 * @param {uint32_t} cntOFF 关计数时间ms
 * @return {*}
 */
void BEEP_Square(uint8_t times, uint32_t cntON, uint32_t cntOFF)
{
    BeepEnable = 0;
    if (times > BEEP_TASK_NUM / 2)
    {
        times = BEEP_TASK_NUM / 2;
    }
    BEEP_ClearTaskList();
    for (uint8_t i = 0; i < times; i++)
    {
        BEEP_AppendTaskList(BEEP_GPIO_ON, cntON);
        BEEP_AppendTaskList(BEEP_GPIO_OFF, cntOFF);
    }
    BeepEnable = 1;
}
