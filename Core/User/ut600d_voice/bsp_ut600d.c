/*
 * @Author: your name
 * @Date: 2021-09-22 15:53:16
 * @LastEditTime: 2021-09-26 12:06:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \simple_7inches_v2\Core\User\ut600d_voice\bsp_ut600d.c
 */

#include "bsp_ut600d.h"
#include <string.h>

static uint32_t sysFreq = 0;
static UT600D_SlaveModeTypdef SlaveMode = {.bits = BITS12, .frame = 0};
static uint8_t selectTGIndex = 0;
static UT600D_VoiceListTypdef VoiceTaskList[VOICE_LIST_NUM];
static uint8_t startPlayVL = 0;
/**
 * @description: 初始化UT600D接口
 * @param {*}
 * @return {*}
 */
void UT600D_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    UT600D_DATA_GPIO_CLK_ENABLE();
    UT600D_BUSY_GPIO_CLK_ENABLE();
    UT600D_CLK_GPIO_CLK_ENABLE();
    UT600D_RST_GPIO_CLK_ENABLE();

    HAL_GPIO_WritePin(UT600D_RST_GPIO_Port, UT600D_RST_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, UT600D_CLK_Pin | UT600D_DATA_Pin, GPIO_PIN_SET);

    /*Configure GPIO pins : PBPin PBPin PBPin */
    GPIO_InitStruct.Pin = UT600D_RST_Pin | UT600D_CLK_Pin | UT600D_DATA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = UT600D_BUSY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(UT600D_BUSY_GPIO_Port, &GPIO_InitStruct);

    HAL_GPIO_WritePin(UT600D_RST_GPIO_Port, UT600D_RST_Pin, GPIO_PIN_SET);

    sysFreq = HAL_RCC_GetSysClockFreq();
}

/**
 * @description: UT600D用us延迟,根据运行一条汇编语句的时间进行推算
 * @param {uint32_t} time 1us
 * @return {*}
 */
static void UT600D_Delay_us(uint32_t time)
{
    uint32_t usCnt = sysFreq / 1000000;
    usCnt *= time;
    usCnt /= 4; //判断，自减操作转化为汇编语言的语句数量比
    while (usCnt--)
    {
    }
}

/**
 * @description: 发送相应指令
 * @param {uint32_t} data 指令内容
 * @return {*}
 */
static void UT600D_SendCommand(uint32_t data)
{
    switch (SlaveMode.bits)
    {
    case BITS12:
        SlaveMode.frame = 0x0800;
        break;
    case BITS8:
        SlaveMode.frame = 0x0080;
        break;
    case BITS4:
        SlaveMode.frame = 0x0008;
        break;
    default:
        SlaveMode.frame = 0x0800;
        break;
    }
    HAL_GPIO_WritePin(UT600D_CLK_GPIO_Port, UT600D_CLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(UT600D_DATA_GPIO_Port, UT600D_DATA_Pin, GPIO_PIN_SET);
    UT600D_Delay_us(100);
    __disable_irq();
    /*weak up time < 2us  启动帧*/
    HAL_GPIO_WritePin(UT600D_CLK_GPIO_Port, UT600D_CLK_Pin, GPIO_PIN_RESET);
    UT600D_Delay_us(1); //实测约1.2us
    HAL_GPIO_WritePin(UT600D_CLK_GPIO_Port, UT600D_CLK_Pin, GPIO_PIN_SET);
    /*reset time >= 200us*/
    UT600D_Delay_us(250);

    /*发送具体内容 MSB*/
    for (uint8_t i = 0; i < SlaveMode.bits; i++)
    {
        if (data & (SlaveMode.frame >> i))
        {
            HAL_GPIO_WritePin(UT600D_DATA_GPIO_Port, UT600D_DATA_Pin, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(UT600D_DATA_GPIO_Port, UT600D_DATA_Pin, GPIO_PIN_RESET);
        }
        UT600D_Delay_us(125);

        /*hold time >= 200us 低电平数据有效*/
        HAL_GPIO_WritePin(UT600D_CLK_GPIO_Port, UT600D_CLK_Pin, GPIO_PIN_RESET);
        UT600D_Delay_us(250);
        HAL_GPIO_WritePin(UT600D_CLK_GPIO_Port, UT600D_CLK_Pin, GPIO_PIN_SET);

        UT600D_Delay_us(125);
    }
    HAL_GPIO_WritePin(UT600D_CLK_GPIO_Port, UT600D_CLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(UT600D_DATA_GPIO_Port, UT600D_DATA_Pin, GPIO_PIN_SET);

    __enable_irq();
    UT600D_Delay_us(100);
}

/**
 * @description: 暂停播放
 * @param {*}
 * @return {*}
 */
void UT600D_PausePlay(void)
{
    UT600D_StopPlayVoiceList();
    SlaveMode.bits = BITS4;
    UT600D_SendCommand(0x00);
}

/**
 * @description: 继续播放
 * @param {*}
 * @return {*}
 */
void UT600D_ResumePlay(void)
{
    UT600D_StartPlayVoiceList();
    SlaveMode.bits = BITS4;
    UT600D_SendCommand(0x02);
}

/**
 * @description: 停止播放
 * @param {*}
 * @return {*}
 */
void UT600D_StopPlay(void)
{
    UT600D_StopPlayVoiceList();
    SlaveMode.bits = BITS4;
    UT600D_SendCommand(0x03);
}

/**
 * @description: 选择TG组
 * @param {uint8_t} numTG
 * @return {*}
 */
void UT600D_SelectTG(uint8_t numTG)
{
    if (numTG < 1 && numTG > 8)
        return;
    selectTGIndex = numTG;
    SlaveMode.bits = BITS8;
    UT600D_SendCommand(0x60 | numTG);
}

/**
 * @description: 选择TG组内的语音序号
 * @param {uint8_t} numVoice 1-254有效
 * @return {*}
 */
static void UT600D_SelectVoice(uint8_t numVoice)
{
    if (numVoice < 1 && numVoice > 254)
        return;
    SlaveMode.bits = BITS12;
    UT600D_SendCommand(0xA00 | numVoice);
}

/**
 * @description: 播放所选择TG组内的语音序号
 * @param {uint8_t} numTG
 * @return {*}
 */
static void UT600D_PlaySelectVoice(uint8_t numTG)
{
    if (numTG < 1 && numTG > 8)
        return;
    SlaveMode.bits = BITS8;
    UT600D_SendCommand(0x70 | numTG);
}

/**
 * @description: 设置音量大小
 * @param {uint8_t} volume 0-7
 * @return {*}
 */
void UT600D_SetVolume(uint8_t volume)
{
    if (volume > 7)
        return;
    UT600D_PausePlay();
    SlaveMode.bits = BITS8;
    UT600D_SendCommand(0x40 | volume);
    UT600D_ResumePlay();
}

/**
 * @description: 选择语言,查表设置TG区域
 * @param {UT600D_LanguageTypdef} language
 * @return {*}
 */
void UT600D_SelectLanguage(UT600D_LanguageTypdef language)
{
    switch (language)
    {
    case Chinese:
        UT600D_SelectTG(6);
        break;
    case English:
        UT600D_SelectTG(6);
        break;

    default:
        break;
    }
}

/**
 * @description: 播放语音
 * @param {uint16_t} volIdx 语音索引 1-254有效
 * @param {uint8_t} wait 0:不等待当前语音播报完毕，直接打断 1：等待当前语音播报完毕，但会丢掉当前语音播报信息
 * @return {*}
 */
UT600D_StatusTypedef UT600D_PlayVoice(uint16_t volIdx, UT600D_WaitTypdef wait)
{
    if (volIdx < 1 && volIdx > 254)
        return UT600D_OVER;
    if (wait == EN_WAIT)
    {
        if (HAL_GPIO_ReadPin(UT600D_BUSY_GPIO_Port, UT600D_BUSY_Pin) == BUSY_LV)
        {
            return UT600D_BUSY;
        }
    }
    else
    {
        // UT600D_DealVoiceList();
    }
    UT600D_SelectVoice(volIdx);
    UT600D_PlaySelectVoice(selectTGIndex);
    return UT600D_OK;
}

/**
 * @description: 添加语音播放组列表参数
 * @param {UT600D_LanguageTypdef} language
 * @param {uint16_t} volIdx
 * @return {*}
 */
UT600D_StatusTypedef UT600D_AppendVoiceList(UT600D_LanguageTypdef language, uint16_t volIdx, UT600D_WaitTypdef wait)
{
    uint8_t i = 0;
    for (; i < VOICE_LIST_NUM; i++)
    {
        if (VoiceTaskList[i].volIdx == 0)
        {
            VoiceTaskList[i].language = language;
            VoiceTaskList[i].volIdx = volIdx;
            VoiceTaskList[i].wait = wait;
            break;
        }
    }
    if (i == VOICE_LIST_NUM)
    {
        return UT600D_LIST_FULL;
    }

    return UT600D_OK;
}

/**
 * @description: 清除语音播放组列表
 * @param {*}
 * @return {*}
 */
void UT600D_ClearVoiceList(void)
{
    memset(VoiceTaskList, 0, sizeof(VoiceTaskList));
    UT600D_StopPlayVoiceList();
}

/**
 * @description: 开启播放语音播放组列表
 * @param {*}
 * @return {*}
 */
void UT600D_StartPlayVoiceList(void)
{
    startPlayVL = 1;
}

/**
 * @description: 停止播放语音播放组列表
 * @param {*}
 * @return {*}
 */
void UT600D_StopPlayVoiceList(void)
{
    startPlayVL = 0;
}

/**
 * @description: 播放语音播放组列表-在time循环中调用
 * @param {*}
 * @return {*}
 */
void UT600D_DealVoiceList(void)
{
    if (VoiceTaskList[0].volIdx == 0 || startPlayVL == 0)
        return;
    if (VoiceTaskList[0].wait == EN_WAIT)
    {
        if (HAL_GPIO_ReadPin(UT600D_BUSY_GPIO_Port, UT600D_BUSY_Pin) == BUSY_LV)
        {
            return;
        }
    }
    UT600D_SelectLanguage(VoiceTaskList[0].language);

    if (UT600D_PlayVoice(VoiceTaskList[0].volIdx, VoiceTaskList[0].wait) == UT600D_OK)
    {
        /*数组前移*/
        for (uint8_t i = 0; i < VOICE_LIST_NUM - 1; i++)
        {
            memcpy(&VoiceTaskList[i], &VoiceTaskList[i + 1], sizeof(UT600D_VoiceListTypdef));
        }
        memset(&VoiceTaskList[VOICE_LIST_NUM - 1], 0, sizeof(UT600D_VoiceListTypdef));
    }
}