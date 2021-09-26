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
 * @description: ��ʼ��UT600D�ӿ�
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
 * @description: UT600D��us�ӳ�,��������һ���������ʱ���������
 * @param {uint32_t} time 1us
 * @return {*}
 */
static void UT600D_Delay_us(uint32_t time)
{
    uint32_t usCnt = sysFreq / 1000000;
    usCnt *= time;
    usCnt /= 4; //�жϣ��Լ�����ת��Ϊ������Ե����������
    while (usCnt--)
    {
    }
}

/**
 * @description: ������Ӧָ��
 * @param {uint32_t} data ָ������
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
    /*weak up time < 2us  ����֡*/
    HAL_GPIO_WritePin(UT600D_CLK_GPIO_Port, UT600D_CLK_Pin, GPIO_PIN_RESET);
    UT600D_Delay_us(1); //ʵ��Լ1.2us
    HAL_GPIO_WritePin(UT600D_CLK_GPIO_Port, UT600D_CLK_Pin, GPIO_PIN_SET);
    /*reset time >= 200us*/
    UT600D_Delay_us(250);

    /*���;������� MSB*/
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

        /*hold time >= 200us �͵�ƽ������Ч*/
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
 * @description: ��ͣ����
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
 * @description: ��������
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
 * @description: ֹͣ����
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
 * @description: ѡ��TG��
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
 * @description: ѡ��TG���ڵ��������
 * @param {uint8_t} numVoice 1-254��Ч
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
 * @description: ������ѡ��TG���ڵ��������
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
 * @description: ����������С
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
 * @description: ѡ������,�������TG����
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
 * @description: ��������
 * @param {uint16_t} volIdx �������� 1-254��Ч
 * @param {uint8_t} wait 0:���ȴ���ǰ����������ϣ�ֱ�Ӵ�� 1���ȴ���ǰ����������ϣ����ᶪ����ǰ����������Ϣ
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
 * @description: ��������������б����
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
 * @description: ��������������б�
 * @param {*}
 * @return {*}
 */
void UT600D_ClearVoiceList(void)
{
    memset(VoiceTaskList, 0, sizeof(VoiceTaskList));
    UT600D_StopPlayVoiceList();
}

/**
 * @description: �������������������б�
 * @param {*}
 * @return {*}
 */
void UT600D_StartPlayVoiceList(void)
{
    startPlayVL = 1;
}

/**
 * @description: ֹͣ���������������б�
 * @param {*}
 * @return {*}
 */
void UT600D_StopPlayVoiceList(void)
{
    startPlayVL = 0;
}

/**
 * @description: ���������������б�-��timeѭ���е���
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
        /*����ǰ��*/
        for (uint8_t i = 0; i < VOICE_LIST_NUM - 1; i++)
        {
            memcpy(&VoiceTaskList[i], &VoiceTaskList[i + 1], sizeof(UT600D_VoiceListTypdef));
        }
        memset(&VoiceTaskList[VOICE_LIST_NUM - 1], 0, sizeof(UT600D_VoiceListTypdef));
    }
}