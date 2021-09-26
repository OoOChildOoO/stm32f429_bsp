#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32f4xx.h"
#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

extern SPI_HandleTypeDef hspi1;
#define SPI_Handle hspi1

#define SPIFLASH_TEST 1

typedef enum
{
    FLASH_ICON = 0,
    FLASH_MP3 = 1,
    FLASH_PAT = 2,
    FLASH_MSC = 3
} EXFLASH_EnumTypeDef;

typedef struct
{
    uint32_t id;
    uint8_t addMode; //3:默认3字节模式 4:4字节模式
    uint16_t pageSize;
    uint16_t sectorSize;
} EXFLASH_TypeTypeDef;

extern const EXFLASH_TypeTypeDef ExFlashTypeList[];

//W25X系列/Q系列芯片列表
#define W25Q08 0XEF13  //容量为 1M字节,共有  16个Block(64KB),  256个Sector(4KB),  4096Page(256Byte),Add:0-0x100000 (3B Add Mode) 0x4014
#define W25Q16 0XEF14  //容量为 2M字节,共有  32个Block(64KB),  512个Sector(4KB),  8192Page(256Byte),Add:0-0x200000 (3B Add Mode) 0x4015
#define W25Q32 0XEF15  //容量为 4M字节,共有  64个Block(64KB), 1024个Sector(4KB), 16384Page(256Byte),Add:0-0x400000 (3B Add Mode) 0x4016
#define W25Q64 0XEF16  //容量为 8M字节,共有 128个Block(64KB), 2048个Sector(4KB), 32768Page(256Byte),Add:0-0x800000 (3B Add Mode) 0x4017
#define W25Q128 0XEF17 //容量为16M字节,共有 256个Block(64KB), 4096个Sector(4KB), 65536Page(256Byte),Add:0-0x1000000(3B Add Mode) 0x4018
#define W25Q256 0XEF18 //容量为32M字节,共有 512个Block(64KB), 8192个Sector(4KB),131072Page(256Byte),Add:0-0x2000000(3B/4B Add Mode) 0x4019
#define W25M512 0XEF19 //容量为64M字节,共有1024个Block(64KB),16384个Sector(4KB),262144Page(256Byte),Add:0-0x4000000(3B/4B Add Mode)

/* Private typedef -----------------------------------------------------------*/
//#define  sFLASH_ID 0xEF3015 //W25X16
//#define  sFLASH_ID 0xEF4015 //W25Q16
//#define  sFLASH_ID 0XEF4017 //W25Q64
// #define sFLASH_ID 0XEF4018 //W25Q128
// #define sFLASH_ID 0XEF4019 //W25Q256

#define SPI_FLASH_SectorSize 4096
#define SPI_FLASH_PageSize 256
#define SPI_FLASH_PerWritePageSize 256

/* Private define ------------------------------------------------------------*/
/*命令定义-开头*******************************/
#define W25X_WriteEnable 0x06
#define W25X_WriteDisable 0x04
#define W25X_ReadStatusReg 0x05
#define W25X_WriteStatusReg 0x01
#define W25X_ReadData 0x03
#define W25X_FastReadData 0x0B
#define W25X_FastReadDual 0x3B
#define W25X_PageProgram 0x02
#define W25X_BlockErase 0xD8
#define W25X_SectorErase 0x20
#define W25X_ChipErase 0xC7
#define W25X_PowerDown 0xB9
#define W25X_ReleasePowerDown 0xAB
#define W25X_DeviceID 0xAB
#define W25X_ManufactDeviceID 0x90
#define W25X_JedecDeviceID 0x9F
#define W25X_Enter4ByteMode 0xB7
#define W25X_ReadStatusRegister3 0x15

#define WIP_Flag 0x01 /* Write In Progress (WIP) flag */
#define Dummy_Byte 0xFF
/*命令定义-结尾*******************************/

#define SPIx SPI1
#define SPIx_CLK_ENABLE() __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_CS_25Q128_ICON_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define SPIx_CS_25Q128_MP3_GPIO_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()
#define SPIx_CS_25Q128_PAT_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_CS_XT26G_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define SPIx_CS_GPIO_CLK_ENABLE()     \
    do                                \
    {                                 \
        __HAL_RCC_GPIOC_CLK_ENABLE(); \
        __HAL_RCC_GPIOH_CLK_ENABLE(); \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0)

#define SPIx_FORCE_RESET() __HAL_RCC_SPI1_FORCE_RESET()
#define SPIx_RELEASE_RESET() __HAL_RCC_SPI1_RELEASE_RESET()

/* Definition for SPIx Pins */
#define SPIx_SCK_PIN GPIO_PIN_5
#define SPIx_SCK_GPIO_PORT GPIOA
#define SPIx_SCK_AF GPIO_AF5_SPI1
#define SPIx_MISO_PIN GPIO_PIN_6
#define SPIx_MISO_GPIO_PORT GPIOA
#define SPIx_MISO_AF GPIO_AF5_SPI1
#define SPIx_MOSI_PIN GPIO_PIN_7
#define SPIx_MOSI_GPIO_PORT GPIOA
#define SPIx_MOSI_AF GPIO_AF5_SPI1

#define FLASH_XT26G_CS_PIN GPIO_PIN_5
#define FLASH_XT26G_CS_GPIO_PORT GPIOC

#define FLASH_25Q128_ICON_CS_PIN GPIO_PIN_4
#define FLASH_25Q128_ICON_CS_GPIO_PORT GPIOH

#define FLASH_25Q128_MP3_CS_PIN GPIO_PIN_0
#define FLASH_25Q128_MP3_CS_GPIO_PORT GPIOB

#define FLASH_25Q128_PAT_CS_PIN GPIO_PIN_15
#define FLASH_25Q128_PAT_CS_GPIO_PORT GPIOA

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define FLASH_DEBUG_ON 1

#define FLASH_INFO(fmt, arg...) printf("<<-FLASH-INFO->> " fmt "\n", ##arg)
#define FLASH_ERROR(fmt, arg...) printf("<<-FLASH-ERROR->> " fmt "\n", ##arg)
#define FLASH_DEBUG(fmt, arg...)                                        \
    do                                                                  \
    {                                                                   \
        if (FLASH_DEBUG_ON)                                             \
            printf("<<-FLASH-DEBUG->> [%d]" fmt "\n", __LINE__, ##arg); \
    } while (0)

#if SPIFLASH_TEST
typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} TestStatus;
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);
void SPI_FLASH_TEST(EXFLASH_EnumTypeDef wcs);
#endif

void SPI_FLASH_Init(void);

void SPI_FLASH_SectorErase(EXFLASH_EnumTypeDef wcs, uint32_t SectorAddr);
void SPI_FLASH_BulkErase(EXFLASH_EnumTypeDef wcs);
void SPI_FLASH_PageWrite(EXFLASH_EnumTypeDef wcs, uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
void SPI_FLASH_SectorWrite(EXFLASH_EnumTypeDef wcs, uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
void SPI_FLASH_BufferWrite(EXFLASH_EnumTypeDef wcs, uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
void SPI_FLASH_BufferRead(EXFLASH_EnumTypeDef wcs, uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
uint32_t SPI_FLASH_ReadID(EXFLASH_EnumTypeDef wcs);
uint32_t SPI_FLASH_ReadDeviceID(EXFLASH_EnumTypeDef wcs);
void SPI_FLASH_StartReadSequence(EXFLASH_EnumTypeDef wcs, uint32_t ReadAddr);
uint8_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t byte);
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
void SPI_FLASH_WriteEnable(EXFLASH_EnumTypeDef wcs);
void SPI_FLASH_WaitForWriteEnd(EXFLASH_EnumTypeDef wcs);
void SPI_Flash_PowerDown(EXFLASH_EnumTypeDef wcs);
void SPI_Flash_WAKEUP(EXFLASH_EnumTypeDef wcs);
void SPI_FLASH_Mode_Init(EXFLASH_EnumTypeDef wcs);

#endif /* __SPI_FLASH_H */
