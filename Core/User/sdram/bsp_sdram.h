/*
 * @Author: your name
 * @Date: 2021-09-02 15:34:14
 * @LastEditTime: 2021-09-25 15:52:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \simple_7inches_v2\Core\User\sdram\bsp_sdram.h
 */
#ifndef __SDRAM_H
#define __SDRAM_H

#include "stm32f4xx_hal.h"
#include "main.h"

extern SDRAM_HandleTypeDef hsdram1;
#define SDRAM_Handle hsdram1
#define SDRAM_TEST 1

#define SDRAM_CTRL_HAL 0
#define SDRAM_CTRL_MY 1
#define SDRAM_CTRL SDRAM_CTRL_HAL

#define W9825G6KH_6_BANK_NUM 4 //4个扇区
#define W9825G6KH_6_BANK1 0x01u
#define W9825G6KH_6_BANK2 0x02u
#define W9825G6KH_6_BANK3 0x04u
#define W9825G6KH_6_BANK4 0x08u
#define W9825G6KH_6_SIZE (4 * 1024 * 1024 * W9825G6KH_6_BANK_NUM * 2) //32M字节

/*SDRAM 的bank选择*/
#define FMC_BANK_SDRAM FMC_Bank1_SDRAM
#define FMC_COMMAND_TARGET_BANK FMC_SDRAM_CMD_TARGET_BANK1

/**
  * @brief  FMC SDRAM 数据基地址
  */
#define SDRAM_BANK_ADDR ((uint32_t)0xC0000000)

/**
  * @brief  FMC SDRAM 数据宽度
  */
/* #define SDRAM_MEMORY_WIDTH   FMC_SDMemory_Width_8b  */
#define SDRAM_MEMORY_WIDTH FMC_SDRAM_MEM_BUS_WIDTH_16

/**
  * @brief  FMC SDRAM CAS Latency
  */
/* #define SDRAM_CAS_LATENCY   FMC_CAS_Latency_2  */
#define SDRAM_CAS_LATENCY FMC_SDRAM_CAS_LATENCY_3

/**
  * @brief  FMC SDRAM SDCLK时钟分频因子
  */
#define SDCLOCK_PERIOD FMC_SDRAM_CLOCK_PERIOD_2 /* Default configuration used with LCD */
/* #define SDCLOCK_PERIOD    FMC_SDClock_Period_3 */

/**
  * @brief  FMC SDRAM 突发读取特性
  */
#define SDRAM_READBURST FMC_SDRAM_RBURST_DISABLE /* Default configuration used with LCD */
/* #define SDRAM_READBURST    FMC_Read_Burst_Enable  */

/**
  * @brief  FMC SDRAM Bank Remap
  */
/* #define SDRAM_BANK_REMAP */

#define SDRAM_TIMEOUT ((uint32_t)0xFFFF)

/**
  * @brief  FMC SDRAM 模式配置的寄存器相关定义
  */
#define SDRAM_MODEREG_BURST_LENGTH_1 ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2 ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4 ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8 ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2 ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3 ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE ((uint16_t)0x0200)

/*信息输出*/
#define SDRAM_DEBUG_ON 1

#define SDRAM_INFO(fmt, arg...) printf("<<-SDRAM-INFO->> " fmt "\n", ##arg)
#define SDRAM_ERROR(fmt, arg...) printf("<<-SDRAM-ERROR->> " fmt "\n", ##arg)
#define SDRAM_DEBUG(fmt, arg...)                                  \
  do                                                              \
  {                                                               \
    if (SDRAM_DEBUG_ON)                                           \
      printf("<<-SDRAM-DEBUG->> [%d]" fmt "\n", __LINE__, ##arg); \
  } while (0)
/**
  * @}
  */

/** @defgroup STM32429 SDRAM函数
  * @{
  */
void SDRAM_Init(void);
void SDRAM_InitSequence(void);
HAL_StatusTypeDef SDRAM_Load(void *pSrcAddress, void *pDstBuffer, uint32_t BufferSize);
HAL_StatusTypeDef SDRAM_Store(void *pDirAddress, void *pSrcBuffer, uint32_t BufferSize);
HAL_StatusTypeDef SDRAM_Load_8b(uint32_t *pSrcAddress, uint8_t *pDstBuffer, uint32_t BufferSize);
HAL_StatusTypeDef SDRAM_Store_8b(uint32_t *pDirAddress, uint8_t *pSrcBuffer, uint32_t BufferSize);
HAL_StatusTypeDef SDRAM_MemSet_8b(uint32_t *pDirAddress, uint8_t Buffer, uint32_t BufferSize);
HAL_StatusTypeDef SDRAM_Load_16b(uint32_t *pSrcAddress, uint16_t *pDstBuffer, uint32_t BufferSize);
HAL_StatusTypeDef SDRAM_Store_16b(uint32_t *pDirAddress, uint16_t *pSrcBuffer, uint32_t BufferSize);
HAL_StatusTypeDef SDRAM_MemSet_16b(uint32_t *pDirAddress, uint16_t Buffer, uint32_t BufferSize);
HAL_StatusTypeDef SDRAM_Load_32b(uint32_t *pSrcAddress, uint32_t *pDstBuffer, uint32_t BufferSize);
HAL_StatusTypeDef SDRAM_Store_32b(uint32_t *pDirAddress, uint32_t *pSrcBuffer, uint32_t BufferSize);
HAL_StatusTypeDef SDRAM_MemSet_32b(uint32_t *pDirAddress, uint32_t Buffer, uint32_t BufferSize);
HAL_StatusTypeDef SDRAM_EraseBank(uint8_t bank);

#if SDRAM_TEST
uint8_t SDRAM_Test(void);
void MulTestSDRAM(uint16_t doCnt);
#endif

#endif /* __SDRAM_H */
