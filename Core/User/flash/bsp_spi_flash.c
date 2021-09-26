/*
 * @Author: your name
 * @Date: 2021-09-17 15:09:53
 * @LastEditTime: 2021-09-25 16:39:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \simple_7inches_v2\Core\User\flash\bsp_spi_flash.c
 */

#include "bsp_spi_flash.h"

const EXFLASH_TypeTypeDef ExFlashTypeList[] = {
    [0] = {0XEF4019, 4, 256, 4096},
    [1] = {0XEF4019, 4, 256, 4096},
    [2] = {0XEF4019, 4, 256, 4096},
    [3] = {0XEF4017, 3, 256, 4096}};

static volatile uint32_t SPITimeout = SPIT_LONG_TIMEOUT;

static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

/**
 * @description: 串行flash的GPIO初始化
 * @param {*}
 * @return {*}
 */
void SPI_FLASH_GOIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  SPIx_SCK_GPIO_CLK_ENABLE();
  SPIx_MISO_GPIO_CLK_ENABLE();
  SPIx_MOSI_GPIO_CLK_ENABLE();
  SPIx_CS_GPIO_CLK_ENABLE();
  /* Enable SPI clock */
  SPIx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* SPI SCK GPIO pin configuration  */
  GPIO_InitStruct.Pin = SPIx_SCK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = SPIx_SCK_AF;

  HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

  /* SPI MISO GPIO pin configuration  */
  GPIO_InitStruct.Pin = SPIx_MISO_PIN;
  GPIO_InitStruct.Alternate = SPIx_MISO_AF;

  HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

  /* SPI MOSI GPIO pin configuration  */
  GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
  GPIO_InitStruct.Alternate = SPIx_MOSI_AF;
  HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FLASH_XT26G_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(FLASH_XT26G_CS_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FLASH_25Q128_ICON_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(FLASH_25Q128_ICON_CS_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FLASH_25Q128_MP3_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(FLASH_25Q128_MP3_CS_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FLASH_25Q128_PAT_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(FLASH_25Q128_PAT_CS_GPIO_PORT, &GPIO_InitStruct);
}

/**
 * @description: 初始化
 * @param {*}
 * @return {*}
 */
void SPI_FLASH_Init(void)
{
  /*##-1- Configure the SPI peripheral #######################################*/
  SPI_FLASH_GOIO_Init();
  /* Set the SPI parameters */
  SPI_Handle.Instance = SPIx;
  SPI_Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  SPI_Handle.Init.Direction = SPI_DIRECTION_2LINES;
  SPI_Handle.Init.CLKPhase = SPI_PHASE_1EDGE;
  SPI_Handle.Init.CLKPolarity = SPI_POLARITY_LOW;
  SPI_Handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  SPI_Handle.Init.CRCPolynomial = 10;
  SPI_Handle.Init.DataSize = SPI_DATASIZE_8BIT;
  SPI_Handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  SPI_Handle.Init.NSS = SPI_NSS_SOFT;
  SPI_Handle.Init.TIMode = SPI_TIMODE_DISABLE;

  SPI_Handle.Init.Mode = SPI_MODE_MASTER;

  HAL_SPI_Init(&SPI_Handle);

  __HAL_SPI_ENABLE(&SPI_Handle);

  /* 使 SPI_FLASH 进入 4 字节地址模式 */
  SPI_FLASH_Mode_Init(FLASH_ICON);
  SPI_FLASH_Mode_Init(FLASH_MP3);
  SPI_FLASH_Mode_Init(FLASH_PAT);
}

/**
 * @description: 片选开
 * @param {EXFLASH_EnumTypeDef} wcs
 * @return {*}
 */
void SPI_FLASH_CS_LOW(EXFLASH_EnumTypeDef wcs)
{
  switch (wcs)
  {
  case FLASH_ICON:
    HAL_GPIO_WritePin(FLASH_25Q128_ICON_CS_GPIO_PORT, FLASH_25Q128_ICON_CS_PIN, GPIO_PIN_RESET);
    break;
  case FLASH_MP3:
    HAL_GPIO_WritePin(FLASH_25Q128_MP3_CS_GPIO_PORT, FLASH_25Q128_MP3_CS_PIN, GPIO_PIN_RESET);
    break;
  case FLASH_PAT:
    HAL_GPIO_WritePin(FLASH_25Q128_PAT_CS_GPIO_PORT, FLASH_25Q128_PAT_CS_PIN, GPIO_PIN_RESET);
    break;
  case FLASH_MSC:
    HAL_GPIO_WritePin(FLASH_XT26G_CS_GPIO_PORT, FLASH_XT26G_CS_PIN, GPIO_PIN_RESET);
    break;

  default:
    HAL_GPIO_WritePin(FLASH_25Q128_ICON_CS_GPIO_PORT, FLASH_25Q128_ICON_CS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FLASH_25Q128_MP3_CS_GPIO_PORT, FLASH_25Q128_MP3_CS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FLASH_25Q128_PAT_CS_GPIO_PORT, FLASH_25Q128_PAT_CS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FLASH_XT26G_CS_GPIO_PORT, FLASH_XT26G_CS_PIN, GPIO_PIN_SET);
    break;
  }
}

/**
 * @description: 片选关
 * @param {EXFLASH_EnumTypeDef} wcs
 * @return {*}
 */
void SPI_FLASH_CS_HIGH(EXFLASH_EnumTypeDef wcs)
{
  switch (wcs)
  {
  case FLASH_ICON:
    HAL_GPIO_WritePin(FLASH_25Q128_ICON_CS_GPIO_PORT, FLASH_25Q128_ICON_CS_PIN, GPIO_PIN_SET);
    break;
  case FLASH_MP3:
    HAL_GPIO_WritePin(FLASH_25Q128_MP3_CS_GPIO_PORT, FLASH_25Q128_MP3_CS_PIN, GPIO_PIN_SET);
    break;
  case FLASH_PAT:
    HAL_GPIO_WritePin(FLASH_25Q128_PAT_CS_GPIO_PORT, FLASH_25Q128_PAT_CS_PIN, GPIO_PIN_SET);
    break;
  case FLASH_MSC:
    HAL_GPIO_WritePin(FLASH_XT26G_CS_GPIO_PORT, FLASH_XT26G_CS_PIN, GPIO_PIN_SET);
    break;

  default:
    HAL_GPIO_WritePin(FLASH_25Q128_ICON_CS_GPIO_PORT, FLASH_25Q128_ICON_CS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FLASH_25Q128_MP3_CS_GPIO_PORT, FLASH_25Q128_MP3_CS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FLASH_25Q128_PAT_CS_GPIO_PORT, FLASH_25Q128_PAT_CS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FLASH_XT26G_CS_GPIO_PORT, FLASH_XT26G_CS_PIN, GPIO_PIN_SET);
    break;
  }
}

/**
  * @brief  擦除FLASH扇区
  * @param  SectorAddr：要擦除的扇区地址
  * @retval 无
  */
void SPI_FLASH_SectorErase(EXFLASH_EnumTypeDef wcs, uint32_t SectorAddr)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable(wcs);
  SPI_FLASH_WaitForWriteEnd(wcs);
  /* 擦除扇区 */
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW(wcs);
  /* 发送扇区擦除指令*/
  SPI_FLASH_SendByte(W25X_SectorErase);
  /*发送擦除扇区地址的高8位*/
  if (ExFlashTypeList[wcs].addMode == 4)
  {
    SPI_FLASH_SendByte((SectorAddr & 0xFF000000) >> 24);
  }
  /*发送擦除扇区地址的中前8位*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* 发送擦除扇区地址的中后8位 */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* 发送擦除扇区地址的低8位 */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH(wcs);
  /* 等待擦除完毕*/
  SPI_FLASH_WaitForWriteEnd(wcs);
}

/**
  * @brief  擦除FLASH扇区，整片擦除
  * @param  无
  * @retval 无
  */
void SPI_FLASH_BulkErase(EXFLASH_EnumTypeDef wcs)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable(wcs);

  /* 整块 Erase */
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW(wcs);
  /* 发送整块擦除指令*/
  SPI_FLASH_SendByte(W25X_ChipErase);
  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH(wcs);

  /* 等待擦除完毕*/
  SPI_FLASH_WaitForWriteEnd(wcs);
}

/**
  * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度，必须小于等于WritePageSize
  * @retval 无
  */
void SPI_FLASH_PageWrite(EXFLASH_EnumTypeDef wcs, uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
  if (NumByteToWrite == 0)
    return;
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable(wcs);

  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW(wcs);
  /* 写页写指令*/
  SPI_FLASH_SendByte(W25X_PageProgram);

  /*发送写地址的高8位*/
  if (ExFlashTypeList[wcs].addMode == 4)
  {
    SPI_FLASH_SendByte((WriteAddr & 0xFF000000) >> 24);
  }
  /*发送写地址的中前8位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*发送写地址的中后8位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*发送写地址的低8位*/
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  if (NumByteToWrite > ExFlashTypeList[wcs].pageSize)
  {
    NumByteToWrite = ExFlashTypeList[wcs].pageSize;
    FLASH_ERROR("SPI_FLASH_PageWrite too large!");
  }

  /* 写入数据*/
  while (NumByteToWrite--)
  {
    /* 发送当前要写入的字节数据 */
    SPI_FLASH_SendByte(*pBuffer);
    /* 指向下一字节数据 */
    pBuffer++;
  }

  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH(wcs);

  /* 等待写入完毕*/
  SPI_FLASH_WaitForWriteEnd(wcs);
}

/**
 * @description: 对FLASH按片区写入数据
 * @param {EXFLASH_EnumTypeDef} wcs
 * @param {uint8_t} *pBuffer
 * @param {uint32_t} WriteAddr
 * @param {uint32_t} NumByteToWrite
 * @return {*}
 */
void SPI_FLASH_SectorWrite(EXFLASH_EnumTypeDef wcs, uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
  uint8_t tmpBuff[SPI_FLASH_SectorSize]; //原数据缓存
  uint32_t OffsetSector = 0, AddrSecStart = 0;
  uint8_t NumOfPage = ExFlashTypeList[wcs].sectorSize / ExFlashTypeList[wcs].pageSize;
  if (NumByteToWrite == 0)
    return;
  /*求出需要擦除的扇区地址*/
  AddrSecStart = WriteAddr / ExFlashTypeList[wcs].sectorSize;
  AddrSecStart *= ExFlashTypeList[wcs].sectorSize;
  /*相对扇区的偏移地址*/
  OffsetSector = WriteAddr % ExFlashTypeList[wcs].sectorSize;

  SPI_FLASH_BufferRead(wcs, tmpBuff, AddrSecStart, ExFlashTypeList[wcs].sectorSize);
  memcpy(&tmpBuff[OffsetSector], pBuffer, NumByteToWrite);
  SPI_FLASH_SectorErase(wcs, WriteAddr);
  for (uint8_t i = 0; i < NumOfPage; i++)
  {
    SPI_FLASH_PageWrite(wcs, &tmpBuff[i * ExFlashTypeList[wcs].pageSize], AddrSecStart + i * ExFlashTypeList[wcs].pageSize, ExFlashTypeList[wcs].pageSize);
  }
}

/**
  * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度
  * @retval 无
  */
void SPI_FLASH_BufferWrite(EXFLASH_EnumTypeDef wcs, uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
#if 0
  uint8_t NumOfFullPage = 0, NumOfSinglePage = 0, NumOfAllPage = 0, AddrP = 0, countP = 0, temp = 0;
  if (NumByteToWrite == 0)
    return;
  /*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
  AddrP = WriteAddr % ExFlashTypeList[wcs].pageSize;
  /*差count个数据值，刚好可以对齐到页地址*/
  countP = ExFlashTypeList[wcs].pageSize - AddrP;
  /*计算出要写多少整数页*/
  NumOfFullPage = NumByteToWrite / ExFlashTypeList[wcs].pageSize;
  /*mod运算求余，计算出剩余不满一页的字节数*/
  NumOfSinglePage = NumByteToWrite % ExFlashTypeList[wcs].pageSize;

  /* AddrP=0,则WriteAddr 刚好按页对齐 aligned  */
  if (AddrP == 0)
  {
    /* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfFullPage == 0)
    {
      SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      /*先把整数页都写了*/
      while (NumOfFullPage--)
      {
        SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, ExFlashTypeList[wcs].pageSize);
        WriteAddr += ExFlashTypeList[wcs].pageSize;
        pBuffer += ExFlashTypeList[wcs].pageSize;
      }
      /*若有多余的不满一页的数据，把它写完*/
      SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, NumOfSinglePage);
    }
  }
  /* 若地址与 SPI_FLASH_PageSize 不对齐  */
  else
  {
    /* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfFullPage == 0)
    {
      /*当前页剩余的count个位置比NumOfSingle小，写不完*/
      if (NumOfSinglePage > countP)
      {
        temp = NumOfSinglePage - countP;

        /*先写满当前页*/
        SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, countP);
        WriteAddr += countP;
        pBuffer += countP;

        /*再写剩余的数据*/
        SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, temp);
      }
      else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
      {
        SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      /*地址不对齐多出的count分开处理，不加入这个运算*/
      NumByteToWrite -= countP;
      NumOfFullPage = NumByteToWrite / ExFlashTypeList[wcs].pageSize;
      NumOfSinglePage = NumByteToWrite % ExFlashTypeList[wcs].pageSize;

      SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, countP);
      WriteAddr += countP;
      pBuffer += countP;

      /*把整数页都写了*/
      while (NumOfFullPage--)
      {
        SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, ExFlashTypeList[wcs].pageSize);
        WriteAddr += ExFlashTypeList[wcs].pageSize;
        pBuffer += ExFlashTypeList[wcs].pageSize;
      }
      /*若有多余的不满一页的数据，把它写完*/
      if (NumOfSinglePage != 0)
      {
        SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, NumOfSinglePage);
      }
    }
  }
#else
  uint16_t NumOfFullSec = 0, NumOfSingleSec = 0, AddrSec = 0, countSec = 0;
  if (NumByteToWrite == 0)
    return;
  /*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
  AddrSec = WriteAddr % ExFlashTypeList[wcs].sectorSize;
  /*差count个数据值，刚好可以对齐到页地址*/
  countSec = ExFlashTypeList[wcs].sectorSize - AddrSec;
  /*计算出要写多少整数页*/
  NumOfFullSec = NumByteToWrite / ExFlashTypeList[wcs].sectorSize;
  /*mod运算求余，计算出剩余不满一页的字节数*/
  NumOfSingleSec = NumByteToWrite % ExFlashTypeList[wcs].sectorSize;

  /* AddrSec=0,则WriteAddr 刚好按页对齐 aligned  */
  if (AddrSec == 0)
  {
    /* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfFullSec == 0)
    {
      SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      /*先把整数页都写了*/
      while (NumOfFullSec--)
      {
        SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, ExFlashTypeList[wcs].sectorSize);
        WriteAddr += ExFlashTypeList[wcs].sectorSize;
        pBuffer += ExFlashTypeList[wcs].sectorSize;
      }
      /*若有多余的不满一页的数据，把它写完*/
      SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, NumOfSingleSec);
    }
  }
  /* 若地址与 SPI_FLASH_PageSize 不对齐  */
  else
  {
    /* NumByteToWrite < SPI_FLASH_SectorSize */
    if (NumOfFullSec == 0)
    {
      /*当前页剩余的count个位置比NumOfSingle小，写不完*/
      if (NumOfSingleSec > countSec)
      {
        uint16_t temp = 0;
        temp = NumOfSingleSec - countSec;

        /*先写满当前页*/
        SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, countSec);
        WriteAddr += countSec;
        pBuffer += countSec;

        /*再写剩余的数据*/
        SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, temp);
      }
      else /*当前页剩余的count个位置能写完 NumOfSingle 个数据*/
      {
        SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_SectorSize */
    {
      /*地址不对齐多出的count分开处理，不加入这个运算*/
      NumByteToWrite -= countSec;
      NumOfFullSec = NumByteToWrite / ExFlashTypeList[wcs].pageSize;
      NumOfSingleSec = NumByteToWrite % ExFlashTypeList[wcs].pageSize;

      SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, countSec);
      WriteAddr += countSec;
      pBuffer += countSec;

      /*把整数页都写了*/
      while (NumOfFullSec--)
      {
        SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, ExFlashTypeList[wcs].pageSize);
        WriteAddr += ExFlashTypeList[wcs].pageSize;
        pBuffer += ExFlashTypeList[wcs].pageSize;
      }
      /*若有多余的不满一页的数据，把它写完*/
      if (NumOfSingleSec != 0)
      {
        SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, NumOfSingleSec);
      }
    }
  }
#endif
}

/**
  * @brief  读取FLASH数据
  * @param 	pBuffer，存储读出数据的指针
  * @param   ReadAddr，读取地址
  * @param   NumByteToRead，读取数据长度
  * @retval 无
  */
void SPI_FLASH_BufferRead(EXFLASH_EnumTypeDef wcs, uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW(wcs);

  /* 发送 读 指令 */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* 发送 读 地址高8位 */
  if (ExFlashTypeList[wcs].addMode == 4)
  {
    SPI_FLASH_SendByte((ReadAddr & 0xFF000000) >> 24);
  }
  /* 发送 读 地址中前8位 */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* 发送 读 地址中后8位 */
  SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
  /* 发送 读 地址低8位 */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  /* 读取数据 */
  while (NumByteToRead--)
  {
    /* 读取一个字节*/
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* 指向下一个字节缓冲区 */
    pBuffer++;
  }

  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH(wcs);
}

/**
  * @brief  读取FLASH ID
  * @param 	无
  * @retval FLASH ID
  */
uint32_t SPI_FLASH_ReadID(EXFLASH_EnumTypeDef wcs)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* 开始通讯：CS低电平 */
  SPI_FLASH_CS_LOW(wcs);

  /* 发送JEDEC指令，读取ID */
  SPI_FLASH_SendByte(W25X_JedecDeviceID);

  /* 读取一个字节数据 */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 读取一个字节数据 */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 读取一个字节数据 */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 停止通讯：CS高电平 */
  SPI_FLASH_CS_HIGH(wcs);

  /*把数据组合起来，作为函数的返回值*/
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

/**
  * @brief  读取FLASH Device ID
  * @param 	无
  * @retval FLASH Device ID
  */
uint32_t SPI_FLASH_ReadDeviceID(EXFLASH_EnumTypeDef wcs)
{
  uint32_t Temp = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW(wcs);

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(W25X_DeviceID);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  Temp = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH(wcs);

  return Temp;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(EXFLASH_EnumTypeDef wcs, uint32_t ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW(wcs);

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}

/**
  * @brief  使用SPI读取一个字节的数据
  * @param  无
  * @retval 返回接收到的数据
  */
uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}

/**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* 等待发送缓冲区为空，TXE事件 */
  while (__HAL_SPI_GET_FLAG(&SPI_Handle, SPI_FLAG_TXE) == RESET)
  {
    if ((SPITimeout--) == 0)
      return SPI_TIMEOUT_UserCallback(0);
  }

  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  WRITE_REG(SPI_Handle.Instance->DR, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* 等待接收缓冲区非空，RXNE事件 */
  while (__HAL_SPI_GET_FLAG(&SPI_Handle, SPI_FLAG_RXNE) == RESET)
  {
    if ((SPITimeout--) == 0)
      return SPI_TIMEOUT_UserCallback(1);
  }

  /* 读取数据寄存器，获取接收缓冲区数据 */
  return READ_REG(SPI_Handle.Instance->DR);
}

/**
  * @brief  使用SPI发送两个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* Loop while DR register in not emplty */
  while (__HAL_SPI_GET_FLAG(&SPI_Handle, SPI_FLAG_TXE) == RESET)
  {
    if ((SPITimeout--) == 0)
      return SPI_TIMEOUT_UserCallback(2);
  }

  /* Send Half Word through the SPIx peripheral */
  WRITE_REG(SPI_Handle.Instance->DR, HalfWord);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* Wait to receive a Half Word */
  while (__HAL_SPI_GET_FLAG(&SPI_Handle, SPI_FLAG_RXNE) == RESET)
  {
    if ((SPITimeout--) == 0)
      return SPI_TIMEOUT_UserCallback(3);
  }
  /* Return the Half Word read from the SPI bus */
  return READ_REG(SPI_Handle.Instance->DR);
}

/**
  * @brief  向FLASH发送 写使能 命令
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(EXFLASH_EnumTypeDef wcs)
{
  /* 通讯开始：CS低 */
  SPI_FLASH_CS_LOW(wcs);

  /* 发送写使能命令*/
  SPI_FLASH_SendByte(W25X_WriteEnable);

  /*通讯结束：CS高 */
  SPI_FLASH_CS_HIGH(wcs);
}

/**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(EXFLASH_EnumTypeDef wcs)
{
  uint8_t FLASH_Status = 0;

  /* 选择 FLASH: CS 低 */
  SPI_FLASH_CS_LOW(wcs);

  /* 发送 读状态寄存器 命令 */
  SPI_FLASH_SendByte(W25X_ReadStatusReg);

  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 若FLASH忙碌，则等待 */
  do
  {
    /* 读取FLASH芯片的状态寄存器 */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);

    {
      if ((SPITimeout--) == 0)
      {
        SPI_TIMEOUT_UserCallback(4);
        return;
      }
    }
  } while ((FLASH_Status & WIP_Flag) == SET); /* 正在写入标志 */

  /* 停止信号  FLASH: CS 高 */
  SPI_FLASH_CS_HIGH(wcs);
}

//进入掉电模式
void SPI_Flash_PowerDown(EXFLASH_EnumTypeDef wcs)
{
  /* 选择 FLASH: CS 低 */
  SPI_FLASH_CS_LOW(wcs);

  /* 发送 掉电 命令 */
  SPI_FLASH_SendByte(W25X_PowerDown);

  /* 停止信号  FLASH: CS 高 */
  SPI_FLASH_CS_HIGH(wcs);
}

//唤醒
void SPI_Flash_WAKEUP(EXFLASH_EnumTypeDef wcs)
{
  /*选择 FLASH: CS 低 */
  SPI_FLASH_CS_LOW(wcs);

  /* 发上 上电 命令 */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);

  /* 停止信号 FLASH: CS 高 */
  SPI_FLASH_CS_HIGH(wcs); //等待TRES1
}

/**
  * @brief  使 SPI_FLASH 进入 4 字节地址模式
  * @param  none
  * @retval none
  */
void SPI_FLASH_Mode_Init(EXFLASH_EnumTypeDef wcs)
{
  uint8_t Temp;
  if (ExFlashTypeList[wcs].addMode != 4)
  {
    return;
  }
  /*选择 FLASH: CS 低 */
  SPI_FLASH_CS_LOW(wcs);

  /* 发送状态寄存器 3 命令 */
  SPI_FLASH_SendByte(W25X_ReadStatusRegister3);

  Temp = SPI_FLASH_SendByte(Dummy_Byte);

  /* 停止信号 FLASH: CS 高 */
  SPI_FLASH_CS_HIGH(wcs);

  if ((Temp & 0x01) == 0)
  {
    /*选择 FLASH: CS 低 */
    SPI_FLASH_CS_LOW(wcs);

    /* 进入4字节模式 */
    SPI_FLASH_SendByte(W25X_Enter4ByteMode);

    /* 停止信号 FLASH: CS 高 */
    SPI_FLASH_CS_HIGH(wcs);
  }
}

/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  FLASH_ERROR("SPI 等待超时!errorCode = %d", errorCode);
  return 0;
}

#if SPIFLASH_TEST

/*
 * 函数名：Buffercmp
 * 描述  ：比较两个缓冲区中的数据是否相等
 * 输入  ：-pBuffer1     src缓冲区指针
 *         -pBuffer2     dst缓冲区指针
 *         -BufferLength 缓冲区长度
 * 输出  ：无
 * 返回  ：-PASSED pBuffer1 等于   pBuffer2
 *         -FAILED pBuffer1 不同于 pBuffer2
 */
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}

#define FLASH_WriteAddress 0x00000
#define FLASH_ReadAddress FLASH_WriteAddress
#define FLASH_SectorToErase FLASH_WriteAddress

/* 获取缓冲区的长度 */
#define countof(a) (sizeof(a) / sizeof(*(a)))
#define BufferSize (countof(Tx_Buffer) - 1)

/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "W25QXXX SPI FLASH TEST*********123466qwerty";
uint8_t Rx_Buffer[BufferSize];

void SPI_FLASH_TEST(EXFLASH_EnumTypeDef wcs)
{
  //读取的ID存储位置
  volatile uint32_t DeviceID = 0;
  volatile uint32_t FlashID = 0;
  volatile TestStatus TransferStatus1 = FAILED;

  printf("\r\n片选：%d\r\n", wcs);

  SPI_Flash_WAKEUP(wcs);

  /* 获取 Flash Device ID */
  DeviceID = SPI_FLASH_ReadDeviceID(wcs);

  HAL_Delay(200);

  /* 获取 SPI Flash ID */
  FlashID = SPI_FLASH_ReadID(wcs);

  printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);

  /* 检验 SPI Flash ID */
  if (FlashID == ExFlashTypeList[wcs].id)
  {
    printf("\r\n检测到SPI FLASH 0x%x = ", FlashID);
    switch (FlashID)
    {
    case 0XEF4019:
      printf("W25Q256 \r\n");
      break;
    case 0XEF4018:
      printf("W25Q128 \r\n");
      break;
    case 0XEF4017:
      printf("W25Q64 \r\n");
      break;
    case 0xEF4015:
      printf("W25Q32 \r\n");
      break;
    case 0xEF4014:
      printf("W25Q16 \r\n");
      break;
    case 0xEF4013:
      printf("W25Q08 \r\n");
      break;

    default:
      printf(" \r\n");
      break;
    }

    /* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
    SPI_FLASH_SectorErase(wcs, FLASH_SectorToErase);

    /* 将发送缓冲区的数据写到flash中 */
    SPI_FLASH_BufferWrite(wcs, Tx_Buffer, FLASH_WriteAddress, BufferSize);
    printf("\r\n写入的数据为：\r\n%s", Tx_Buffer);

    SPI_FLASH_BufferWrite(wcs, Tx_Buffer, 100, BufferSize);
    printf("\r\n写入的数据2为：\r\n%s", Tx_Buffer);

    SPI_FLASH_BufferWrite(wcs, Tx_Buffer, 4080, BufferSize);
    printf("\r\n写入的数据3为：\r\n%s", Tx_Buffer);


    /* 将刚刚写入的数据读出来放到接收缓冲区中 */
    SPI_FLASH_BufferRead(wcs, Rx_Buffer, FLASH_ReadAddress, BufferSize);
    printf("\r\n读出的数据为：\r\n%s", Rx_Buffer);

    /* 检查写入的数据与读出的数据是否相等 */
    TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

    /* 将刚刚写入的数据读出来放到接收缓冲区中 */
    SPI_FLASH_BufferRead(wcs, Rx_Buffer, 100, BufferSize);
    printf("\r\n读出的数据2为：\r\n%s", Rx_Buffer);

    /* 检查写入的数据与读出的数据是否相等 */
    TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

    /* 将刚刚写入的数据读出来放到接收缓冲区中 */
    SPI_FLASH_BufferRead(wcs, Rx_Buffer, 4080, BufferSize);
    printf("\r\n读出的数据3为：\r\n%s", Rx_Buffer);

    /* 检查写入的数据与读出的数据是否相等 */
    TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

    if (PASSED == TransferStatus1)
    {
      printf("\r\n测试成功!\n\r\n\r");
    }
    else
    {
      printf("\r\n测试失败!\n\r\n\r");
    }
  }
  else
  {
    printf("\r\n获取不到 ID!\n\r\n\r");
  }

  // SPI_Flash_PowerDown(wcs);
}
#endif
/*********************************************END OF FILE**********************/
