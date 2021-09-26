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
 * @description: ����flash��GPIO��ʼ��
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
 * @description: ��ʼ��
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

  /* ʹ SPI_FLASH ���� 4 �ֽڵ�ַģʽ */
  SPI_FLASH_Mode_Init(FLASH_ICON);
  SPI_FLASH_Mode_Init(FLASH_MP3);
  SPI_FLASH_Mode_Init(FLASH_PAT);
}

/**
 * @description: Ƭѡ��
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
 * @description: Ƭѡ��
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
  * @brief  ����FLASH����
  * @param  SectorAddr��Ҫ������������ַ
  * @retval ��
  */
void SPI_FLASH_SectorErase(EXFLASH_EnumTypeDef wcs, uint32_t SectorAddr)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable(wcs);
  SPI_FLASH_WaitForWriteEnd(wcs);
  /* �������� */
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW(wcs);
  /* ������������ָ��*/
  SPI_FLASH_SendByte(W25X_SectorErase);
  /*���Ͳ���������ַ�ĸ�8λ*/
  if (ExFlashTypeList[wcs].addMode == 4)
  {
    SPI_FLASH_SendByte((SectorAddr & 0xFF000000) >> 24);
  }
  /*���Ͳ���������ַ����ǰ8λ*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* ���Ͳ���������ַ���к�8λ */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* ���Ͳ���������ַ�ĵ�8λ */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH(wcs);
  /* �ȴ��������*/
  SPI_FLASH_WaitForWriteEnd(wcs);
}

/**
  * @brief  ����FLASH��������Ƭ����
  * @param  ��
  * @retval ��
  */
void SPI_FLASH_BulkErase(EXFLASH_EnumTypeDef wcs)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable(wcs);

  /* ���� Erase */
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW(wcs);
  /* �����������ָ��*/
  SPI_FLASH_SendByte(W25X_ChipErase);
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH(wcs);

  /* �ȴ��������*/
  SPI_FLASH_WaitForWriteEnd(wcs);
}

/**
  * @brief  ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ��ȣ�����С�ڵ���WritePageSize
  * @retval ��
  */
void SPI_FLASH_PageWrite(EXFLASH_EnumTypeDef wcs, uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
  if (NumByteToWrite == 0)
    return;
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable(wcs);

  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW(wcs);
  /* дҳдָ��*/
  SPI_FLASH_SendByte(W25X_PageProgram);

  /*����д��ַ�ĸ�8λ*/
  if (ExFlashTypeList[wcs].addMode == 4)
  {
    SPI_FLASH_SendByte((WriteAddr & 0xFF000000) >> 24);
  }
  /*����д��ַ����ǰ8λ*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*����д��ַ���к�8λ*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*����д��ַ�ĵ�8λ*/
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  if (NumByteToWrite > ExFlashTypeList[wcs].pageSize)
  {
    NumByteToWrite = ExFlashTypeList[wcs].pageSize;
    FLASH_ERROR("SPI_FLASH_PageWrite too large!");
  }

  /* д������*/
  while (NumByteToWrite--)
  {
    /* ���͵�ǰҪд����ֽ����� */
    SPI_FLASH_SendByte(*pBuffer);
    /* ָ����һ�ֽ����� */
    pBuffer++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH(wcs);

  /* �ȴ�д�����*/
  SPI_FLASH_WaitForWriteEnd(wcs);
}

/**
 * @description: ��FLASH��Ƭ��д������
 * @param {EXFLASH_EnumTypeDef} wcs
 * @param {uint8_t} *pBuffer
 * @param {uint32_t} WriteAddr
 * @param {uint32_t} NumByteToWrite
 * @return {*}
 */
void SPI_FLASH_SectorWrite(EXFLASH_EnumTypeDef wcs, uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
  uint8_t tmpBuff[SPI_FLASH_SectorSize]; //ԭ���ݻ���
  uint32_t OffsetSector = 0, AddrSecStart = 0;
  uint8_t NumOfPage = ExFlashTypeList[wcs].sectorSize / ExFlashTypeList[wcs].pageSize;
  if (NumByteToWrite == 0)
    return;
  /*�����Ҫ������������ַ*/
  AddrSecStart = WriteAddr / ExFlashTypeList[wcs].sectorSize;
  AddrSecStart *= ExFlashTypeList[wcs].sectorSize;
  /*���������ƫ�Ƶ�ַ*/
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
  * @brief  ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param  WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ���
  * @retval ��
  */
void SPI_FLASH_BufferWrite(EXFLASH_EnumTypeDef wcs, uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
#if 0
  uint8_t NumOfFullPage = 0, NumOfSinglePage = 0, NumOfAllPage = 0, AddrP = 0, countP = 0, temp = 0;
  if (NumByteToWrite == 0)
    return;
  /*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
  AddrP = WriteAddr % ExFlashTypeList[wcs].pageSize;
  /*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
  countP = ExFlashTypeList[wcs].pageSize - AddrP;
  /*�����Ҫд��������ҳ*/
  NumOfFullPage = NumByteToWrite / ExFlashTypeList[wcs].pageSize;
  /*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
  NumOfSinglePage = NumByteToWrite % ExFlashTypeList[wcs].pageSize;

  /* AddrP=0,��WriteAddr �պð�ҳ���� aligned  */
  if (AddrP == 0)
  {
    /* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfFullPage == 0)
    {
      SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      /*�Ȱ�����ҳ��д��*/
      while (NumOfFullPage--)
      {
        SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, ExFlashTypeList[wcs].pageSize);
        WriteAddr += ExFlashTypeList[wcs].pageSize;
        pBuffer += ExFlashTypeList[wcs].pageSize;
      }
      /*���ж���Ĳ���һҳ�����ݣ�����д��*/
      SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, NumOfSinglePage);
    }
  }
  /* ����ַ�� SPI_FLASH_PageSize ������  */
  else
  {
    /* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfFullPage == 0)
    {
      /*��ǰҳʣ���count��λ�ñ�NumOfSingleС��д����*/
      if (NumOfSinglePage > countP)
      {
        temp = NumOfSinglePage - countP;

        /*��д����ǰҳ*/
        SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, countP);
        WriteAddr += countP;
        pBuffer += countP;

        /*��дʣ�������*/
        SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, temp);
      }
      else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
      {
        SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      /*��ַ����������count�ֿ������������������*/
      NumByteToWrite -= countP;
      NumOfFullPage = NumByteToWrite / ExFlashTypeList[wcs].pageSize;
      NumOfSinglePage = NumByteToWrite % ExFlashTypeList[wcs].pageSize;

      SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, countP);
      WriteAddr += countP;
      pBuffer += countP;

      /*������ҳ��д��*/
      while (NumOfFullPage--)
      {
        SPI_FLASH_PageWrite(wcs, pBuffer, WriteAddr, ExFlashTypeList[wcs].pageSize);
        WriteAddr += ExFlashTypeList[wcs].pageSize;
        pBuffer += ExFlashTypeList[wcs].pageSize;
      }
      /*���ж���Ĳ���һҳ�����ݣ�����д��*/
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
  /*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
  AddrSec = WriteAddr % ExFlashTypeList[wcs].sectorSize;
  /*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
  countSec = ExFlashTypeList[wcs].sectorSize - AddrSec;
  /*�����Ҫд��������ҳ*/
  NumOfFullSec = NumByteToWrite / ExFlashTypeList[wcs].sectorSize;
  /*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
  NumOfSingleSec = NumByteToWrite % ExFlashTypeList[wcs].sectorSize;

  /* AddrSec=0,��WriteAddr �պð�ҳ���� aligned  */
  if (AddrSec == 0)
  {
    /* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfFullSec == 0)
    {
      SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      /*�Ȱ�����ҳ��д��*/
      while (NumOfFullSec--)
      {
        SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, ExFlashTypeList[wcs].sectorSize);
        WriteAddr += ExFlashTypeList[wcs].sectorSize;
        pBuffer += ExFlashTypeList[wcs].sectorSize;
      }
      /*���ж���Ĳ���һҳ�����ݣ�����д��*/
      SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, NumOfSingleSec);
    }
  }
  /* ����ַ�� SPI_FLASH_PageSize ������  */
  else
  {
    /* NumByteToWrite < SPI_FLASH_SectorSize */
    if (NumOfFullSec == 0)
    {
      /*��ǰҳʣ���count��λ�ñ�NumOfSingleС��д����*/
      if (NumOfSingleSec > countSec)
      {
        uint16_t temp = 0;
        temp = NumOfSingleSec - countSec;

        /*��д����ǰҳ*/
        SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, countSec);
        WriteAddr += countSec;
        pBuffer += countSec;

        /*��дʣ�������*/
        SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, temp);
      }
      else /*��ǰҳʣ���count��λ����д�� NumOfSingle ������*/
      {
        SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_SectorSize */
    {
      /*��ַ����������count�ֿ������������������*/
      NumByteToWrite -= countSec;
      NumOfFullSec = NumByteToWrite / ExFlashTypeList[wcs].pageSize;
      NumOfSingleSec = NumByteToWrite % ExFlashTypeList[wcs].pageSize;

      SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, countSec);
      WriteAddr += countSec;
      pBuffer += countSec;

      /*������ҳ��д��*/
      while (NumOfFullSec--)
      {
        SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, ExFlashTypeList[wcs].pageSize);
        WriteAddr += ExFlashTypeList[wcs].pageSize;
        pBuffer += ExFlashTypeList[wcs].pageSize;
      }
      /*���ж���Ĳ���һҳ�����ݣ�����д��*/
      if (NumOfSingleSec != 0)
      {
        SPI_FLASH_SectorWrite(wcs, pBuffer, WriteAddr, NumOfSingleSec);
      }
    }
  }
#endif
}

/**
  * @brief  ��ȡFLASH����
  * @param 	pBuffer���洢�������ݵ�ָ��
  * @param   ReadAddr����ȡ��ַ
  * @param   NumByteToRead����ȡ���ݳ���
  * @retval ��
  */
void SPI_FLASH_BufferRead(EXFLASH_EnumTypeDef wcs, uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW(wcs);

  /* ���� �� ָ�� */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* ���� �� ��ַ��8λ */
  if (ExFlashTypeList[wcs].addMode == 4)
  {
    SPI_FLASH_SendByte((ReadAddr & 0xFF000000) >> 24);
  }
  /* ���� �� ��ַ��ǰ8λ */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* ���� �� ��ַ�к�8λ */
  SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
  /* ���� �� ��ַ��8λ */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  /* ��ȡ���� */
  while (NumByteToRead--)
  {
    /* ��ȡһ���ֽ�*/
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* ָ����һ���ֽڻ����� */
    pBuffer++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH(wcs);
}

/**
  * @brief  ��ȡFLASH ID
  * @param 	��
  * @retval FLASH ID
  */
uint32_t SPI_FLASH_ReadID(EXFLASH_EnumTypeDef wcs)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* ��ʼͨѶ��CS�͵�ƽ */
  SPI_FLASH_CS_LOW(wcs);

  /* ����JEDECָ���ȡID */
  SPI_FLASH_SendByte(W25X_JedecDeviceID);

  /* ��ȡһ���ֽ����� */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* ��ȡһ���ֽ����� */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* ��ȡһ���ֽ����� */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

  /* ֹͣͨѶ��CS�ߵ�ƽ */
  SPI_FLASH_CS_HIGH(wcs);

  /*�����������������Ϊ�����ķ���ֵ*/
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

/**
  * @brief  ��ȡFLASH Device ID
  * @param 	��
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
  * @brief  ʹ��SPI��ȡһ���ֽڵ�����
  * @param  ��
  * @retval ���ؽ��յ�������
  */
uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}

/**
  * @brief  ʹ��SPI����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (__HAL_SPI_GET_FLAG(&SPI_Handle, SPI_FLAG_TXE) == RESET)
  {
    if ((SPITimeout--) == 0)
      return SPI_TIMEOUT_UserCallback(0);
  }

  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  WRITE_REG(SPI_Handle.Instance->DR, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (__HAL_SPI_GET_FLAG(&SPI_Handle, SPI_FLAG_RXNE) == RESET)
  {
    if ((SPITimeout--) == 0)
      return SPI_TIMEOUT_UserCallback(1);
  }

  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
  return READ_REG(SPI_Handle.Instance->DR);
}

/**
  * @brief  ʹ��SPI���������ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
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
  * @brief  ��FLASH���� дʹ�� ����
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(EXFLASH_EnumTypeDef wcs)
{
  /* ͨѶ��ʼ��CS�� */
  SPI_FLASH_CS_LOW(wcs);

  /* ����дʹ������*/
  SPI_FLASH_SendByte(W25X_WriteEnable);

  /*ͨѶ������CS�� */
  SPI_FLASH_CS_HIGH(wcs);
}

/**
  * @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(EXFLASH_EnumTypeDef wcs)
{
  uint8_t FLASH_Status = 0;

  /* ѡ�� FLASH: CS �� */
  SPI_FLASH_CS_LOW(wcs);

  /* ���� ��״̬�Ĵ��� ���� */
  SPI_FLASH_SendByte(W25X_ReadStatusReg);

  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* ��FLASHæµ����ȴ� */
  do
  {
    /* ��ȡFLASHоƬ��״̬�Ĵ��� */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);

    {
      if ((SPITimeout--) == 0)
      {
        SPI_TIMEOUT_UserCallback(4);
        return;
      }
    }
  } while ((FLASH_Status & WIP_Flag) == SET); /* ����д���־ */

  /* ֹͣ�ź�  FLASH: CS �� */
  SPI_FLASH_CS_HIGH(wcs);
}

//�������ģʽ
void SPI_Flash_PowerDown(EXFLASH_EnumTypeDef wcs)
{
  /* ѡ�� FLASH: CS �� */
  SPI_FLASH_CS_LOW(wcs);

  /* ���� ���� ���� */
  SPI_FLASH_SendByte(W25X_PowerDown);

  /* ֹͣ�ź�  FLASH: CS �� */
  SPI_FLASH_CS_HIGH(wcs);
}

//����
void SPI_Flash_WAKEUP(EXFLASH_EnumTypeDef wcs)
{
  /*ѡ�� FLASH: CS �� */
  SPI_FLASH_CS_LOW(wcs);

  /* ���� �ϵ� ���� */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);

  /* ֹͣ�ź� FLASH: CS �� */
  SPI_FLASH_CS_HIGH(wcs); //�ȴ�TRES1
}

/**
  * @brief  ʹ SPI_FLASH ���� 4 �ֽڵ�ַģʽ
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
  /*ѡ�� FLASH: CS �� */
  SPI_FLASH_CS_LOW(wcs);

  /* ����״̬�Ĵ��� 3 ���� */
  SPI_FLASH_SendByte(W25X_ReadStatusRegister3);

  Temp = SPI_FLASH_SendByte(Dummy_Byte);

  /* ֹͣ�ź� FLASH: CS �� */
  SPI_FLASH_CS_HIGH(wcs);

  if ((Temp & 0x01) == 0)
  {
    /*ѡ�� FLASH: CS �� */
    SPI_FLASH_CS_LOW(wcs);

    /* ����4�ֽ�ģʽ */
    SPI_FLASH_SendByte(W25X_Enter4ByteMode);

    /* ֹͣ�ź� FLASH: CS �� */
    SPI_FLASH_CS_HIGH(wcs);
  }
}

/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d", errorCode);
  return 0;
}

#if SPIFLASH_TEST

/*
 * ��������Buffercmp
 * ����  ���Ƚ������������е������Ƿ����
 * ����  ��-pBuffer1     src������ָ��
 *         -pBuffer2     dst������ָ��
 *         -BufferLength ����������
 * ���  ����
 * ����  ��-PASSED pBuffer1 ����   pBuffer2
 *         -FAILED pBuffer1 ��ͬ�� pBuffer2
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

/* ��ȡ�������ĳ��� */
#define countof(a) (sizeof(a) / sizeof(*(a)))
#define BufferSize (countof(Tx_Buffer) - 1)

/* ���ͻ�������ʼ�� */
uint8_t Tx_Buffer[] = "W25QXXX SPI FLASH TEST*********123466qwerty";
uint8_t Rx_Buffer[BufferSize];

void SPI_FLASH_TEST(EXFLASH_EnumTypeDef wcs)
{
  //��ȡ��ID�洢λ��
  volatile uint32_t DeviceID = 0;
  volatile uint32_t FlashID = 0;
  volatile TestStatus TransferStatus1 = FAILED;

  printf("\r\nƬѡ��%d\r\n", wcs);

  SPI_Flash_WAKEUP(wcs);

  /* ��ȡ Flash Device ID */
  DeviceID = SPI_FLASH_ReadDeviceID(wcs);

  HAL_Delay(200);

  /* ��ȡ SPI Flash ID */
  FlashID = SPI_FLASH_ReadID(wcs);

  printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);

  /* ���� SPI Flash ID */
  if (FlashID == ExFlashTypeList[wcs].id)
  {
    printf("\r\n��⵽SPI FLASH 0x%x = ", FlashID);
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

    /* ������Ҫд��� SPI FLASH ������FLASHд��ǰҪ�Ȳ��� */
    SPI_FLASH_SectorErase(wcs, FLASH_SectorToErase);

    /* �����ͻ�����������д��flash�� */
    SPI_FLASH_BufferWrite(wcs, Tx_Buffer, FLASH_WriteAddress, BufferSize);
    printf("\r\nд�������Ϊ��\r\n%s", Tx_Buffer);

    SPI_FLASH_BufferWrite(wcs, Tx_Buffer, 100, BufferSize);
    printf("\r\nд�������2Ϊ��\r\n%s", Tx_Buffer);

    SPI_FLASH_BufferWrite(wcs, Tx_Buffer, 4080, BufferSize);
    printf("\r\nд�������3Ϊ��\r\n%s", Tx_Buffer);


    /* ���ո�д������ݶ������ŵ����ջ������� */
    SPI_FLASH_BufferRead(wcs, Rx_Buffer, FLASH_ReadAddress, BufferSize);
    printf("\r\n����������Ϊ��\r\n%s", Rx_Buffer);

    /* ���д�������������������Ƿ���� */
    TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

    /* ���ո�д������ݶ������ŵ����ջ������� */
    SPI_FLASH_BufferRead(wcs, Rx_Buffer, 100, BufferSize);
    printf("\r\n����������2Ϊ��\r\n%s", Rx_Buffer);

    /* ���д�������������������Ƿ���� */
    TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

    /* ���ո�д������ݶ������ŵ����ջ������� */
    SPI_FLASH_BufferRead(wcs, Rx_Buffer, 4080, BufferSize);
    printf("\r\n����������3Ϊ��\r\n%s", Rx_Buffer);

    /* ���д�������������������Ƿ���� */
    TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

    if (PASSED == TransferStatus1)
    {
      printf("\r\n���Գɹ�!\n\r\n\r");
    }
    else
    {
      printf("\r\n����ʧ��!\n\r\n\r");
    }
  }
  else
  {
    printf("\r\n��ȡ���� ID!\n\r\n\r");
  }

  // SPI_Flash_PowerDown(wcs);
}
#endif
/*********************************************END OF FILE**********************/
