/*
 * @Author: your name
 * @Date: 2021-09-22 15:53:16
 * @LastEditTime: 2021-09-26 11:25:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \simple_7inches_v2\Core\User\ut600d_voice\bsp_ut600d.c
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_UT600D_H__
#define __BSP_UT600D_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


#define UT600D_RST_Pin GPIO_PIN_3 //P02
#define UT600D_RST_GPIO_Port GPIOB
#define UT600D_RST_GPIO_CLK_ENABLE() __GPIOB_CLK_ENABLE()
#define UT600D_CLK_Pin GPIO_PIN_4 //P03
#define UT600D_CLK_GPIO_Port GPIOB
#define UT600D_CLK_GPIO_CLK_ENABLE() __GPIOB_CLK_ENABLE()
#define UT600D_BUSY_Pin GPIO_PIN_5 //P00
#define UT600D_BUSY_GPIO_Port GPIOB
#define UT600D_BUSY_GPIO_CLK_ENABLE() __GPIOB_CLK_ENABLE()
#define UT600D_DATA_Pin GPIO_PIN_12 //P01
#define UT600D_DATA_GPIO_Port GPIOB
#define UT600D_DATA_GPIO_CLK_ENABLE() __GPIOB_CLK_ENABLE()

#define BUSY_LV GPIO_PIN_SET 

    // #define UT600D_CLK_Pin GPIO_PIN_3 //P02
    // #define UT600D_CLK_GPIO_Port GPIOB
    // #define UT600D_BUSY_Pin GPIO_PIN_4 //P03
    // #define UT600D_BUSY_GPIO_Port GPIOB
    // #define UT600D_DATA_Pin GPIO_PIN_5 //P00
    // #define UT600D_DATA_GPIO_Port GPIOB
    // #define UT600D_RST_Pin GPIO_PIN_12 //P01
    // #define UT600D_RST_GPIO_Port GPIOB
    typedef enum
    {
        UT600D_OK = 0,
        UT600D_BUSY = 1,
        UT600D_LIST_FULL = 2,
        UT600D_OVER = 3,
    } UT600D_StatusTypedef;

    typedef enum
    {
        BITS4 = 4,
        BITS8 = 8,
        BITS12 = 12
    } UT600D_ByteTypedef;
    typedef struct
    {
        UT600D_ByteTypedef bits;
        uint32_t frame;
    } UT600D_SlaveModeTypdef;

    typedef enum
    {
        Chinese = 0,
        English = 1
    } UT600D_LanguageTypdef;

    typedef enum
    {
        DIS_WAIT = 0,
        EN_WAIT = 1
    } UT600D_WaitTypdef;

    typedef struct
    {
        UT600D_LanguageTypdef language;
        uint8_t volIdx;
        UT600D_WaitTypdef wait;
    } UT600D_VoiceListTypdef;

#define VOICE_LIST_NUM 4

    void UT600D_Init(void);
    void UT600D_PausePlay(void);
    void UT600D_ResumePlay(void);
    void UT600D_StopPlay(void);
    void UT600D_SetVolume(uint8_t volume);
    void UT600D_SelectTG(uint8_t numTG);
    void UT600D_SelectLanguage(UT600D_LanguageTypdef language);
    UT600D_StatusTypedef UT600D_PlayVoice(uint16_t volIdx, UT600D_WaitTypdef wait);
    UT600D_StatusTypedef UT600D_AppendVoiceList(UT600D_LanguageTypdef language, uint16_t volIdx, UT600D_WaitTypdef wait);
    void UT600D_ClearVoiceList(void);
    void UT600D_StartPlayVoiceList(void);
    void UT600D_StopPlayVoiceList(void);
    void UT600D_DealVoiceList(void);
#ifdef __cplusplus
}
#endif

#endif /* __BSP_UT600D_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
