/**
  ******************************************************************************
  * @file    bsp_sdram.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   sdramӦ�ú����ӿ�
  ******************************************************************************
  */

#include "bsp_sdram.h"
#include <string.h>
#include <stdio.h>

uint8_t SdramBuff[W9825G6KH_6_SIZE] __attribute__((at(SDRAM_BANK_ADDR)));
static uint8_t SDRAM_Initialized = 0;

/* FMC initialization function */
void SDRAM_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  FMC_SDRAM_TimingTypeDef SdramTiming = {0};
  if (SDRAM_Initialized) {
    return;
  }
  SDRAM_Initialized = 1;

  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_ENABLE();
  // __HAL_RCC_GPIOF_CLK_ENABLE();
  // __HAL_RCC_GPIOC_CLK_ENABLE();
  // __HAL_RCC_GPIOG_CLK_ENABLE();
  // __HAL_RCC_GPIOE_CLK_ENABLE();
  // __HAL_RCC_GPIOD_CLK_ENABLE();

  /** FMC GPIO Configuration
  PF0   ------> FMC_A0
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PC0   ------> FMC_SDNWE
  PC2   ------> FMC_SDNE0
  PC3   ------> FMC_SDCKE0
  PF11   ------> FMC_SDNRAS
  PF12   ------> FMC_A6
  PF13   ------> FMC_A7
  PF14   ------> FMC_A8
  PF15   ------> FMC_A9
  PG0   ------> FMC_A10
  PG1   ------> FMC_A11
  PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10   ------> FMC_D7
  PE11   ------> FMC_D8
  PE12   ------> FMC_D9
  PE13   ------> FMC_D10
  PE14   ------> FMC_D11
  PE15   ------> FMC_D12
  PD8   ------> FMC_D13
  PD9   ------> FMC_D14
  PD10   ------> FMC_D15
  PD14   ------> FMC_D0
  PD15   ------> FMC_D1
  PG2   ------> FMC_A12
  PG4   ------> FMC_BA0
  PG5   ------> FMC_BA1
  PG8   ------> FMC_SDCLK
  PD0   ------> FMC_D2
  PD1   ------> FMC_D3
  PG15   ------> FMC_SDNCAS
  PE0   ------> FMC_NBL0
  PE1   ------> FMC_NBL1
  */
  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_11|GPIO_PIN_12
                          |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 6;
  SdramTiming.WriteRecoveryTime = 2;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* FMC SDRAM �������� */
  /* SDBank:ѡ��洢����*/
  /* ColumnBitsNumber:�е�ַ�߿��: [8:0] */
  /* RowBitsNumber:�е�ַ�߿��: [12:0] */
  /* MemoryDataWidth:�����߿�� */
  /* InternalBankNumber:SDRAM�ڲ�bank����*/
  /* CASLatency:CASǱ���� */
  /* WriteProtection:��ֹд����*/
  /* SDClockPeriod:SDCLKʱ�ӷ�Ƶ���ӣ�SDCLK = HCLK/SDCLOCK_PERIOD*/
  /* ReadBurst:ͻ����ģʽ����*/
  /* ReadPipeDelay:���ӳ����� */

  /* SDRAMʱ����� */
  /* SDRAMʱ��ṹ�壬����SDRAM����������----------------*/
  /* fSDCLK�� 90 Mhz (HCLK/2 :180Mhz/2) 1��ʱ������tSDCLK =1/90MHz=11.11ns*/
  /* tMRD(RSC): min=2 *tSDCLK   LoadToActiveDelay:����ģʽ�Ĵ������������Ч��ˢ������֮����ӳ�*/
  /* tXSR: min=72ns (7x11.11ns)   ExitSelfRefreshDelay:�˳�����ˢ�µ�����Ч����֮����ӳ�*/
  /* tRAS: min=42ns (4x11.11ns) max=100000 (ns)   SelfRefreshTime:����Ч��Ԥ�������֮����ӳ�*/
  /* tRC:  min=60ns (6x11.11ns)   RowCycleDelay:����ˢ�����������������Ч����֮����ӳ�*/
  /* tWR:  min=2 *tSDCLK    WriteRecoveryTime:д�����Ԥ�������֮����ӳ�*/
  /* tRP:  min=15ns (2x11.11ns)   RPDelay:Ԥ���������Ч����֮����ӳ�*/
  /* tRCD: min=15ns (2x11.11ns)   RCDDelay:����Ч���ж�д����֮����ӳ�*/

  SDRAM_InitSequence();
  SDRAM_EraseBank(W9825G6KH_6_BANK1 | W9825G6KH_6_BANK2 | W9825G6KH_6_BANK3 | W9825G6KH_6_BANK4);
  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief  ��SDRAMоƬ���г�ʼ������
  * @param  None. 
  * @retval None.
  */
void SDRAM_InitSequence(void)
{
  FMC_SDRAM_CommandTypeDef Command;
  uint32_t tmpr = 0;
  /* Step 1  ������������ṩ��SDRAM��ʱ��*/
  Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;

  HAL_SDRAM_SendCommand(&SDRAM_Handle, &Command, SDRAM_TIMEOUT);

  /* Step 2  ��������ӳ����� 200us */
  //SDRAM_delay(1);
  HAL_Delay(1);

  /* Step 3  ������������е�bankԤ��� */
  Command.CommandMode = FMC_SDRAM_CMD_PALL;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;

  HAL_SDRAM_SendCommand(&SDRAM_Handle, &Command, SDRAM_TIMEOUT);

  /* Step 4  �����������8���Զ�ˢ������ */
  Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 8;
  Command.ModeRegisterDefinition = 0;

  HAL_SDRAM_SendCommand(&SDRAM_Handle, &Command, SDRAM_TIMEOUT);

  /* Step 5  �����������SRAM�ļ���ģʽ�Ĵ��� */
  tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1 |
         SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |
         SDRAM_MODEREG_CAS_LATENCY_3 |
         SDRAM_MODEREG_OPERATING_MODE_STANDARD |
         SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
  // tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_8 |
  //        SDRAM_MODEREG_BURST_TYPE_INTERLEAVED |
  //        SDRAM_MODEREG_CAS_LATENCY_3 |
  //        SDRAM_MODEREG_OPERATING_MODE_STANDARD |
  //        SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ;

  Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = tmpr;

  HAL_SDRAM_SendCommand(&SDRAM_Handle, &Command, SDRAM_TIMEOUT);

  /* Step 6  �����������FMC��SDRAM���������Զ�ˢ������ */
  //SDRAM refresh rate = T sdram refresh period  / number of rows  =  64ms/8192 = 7.813us
  //count = (SDRAM refresh rate * f SDRAM) - 20 = (7.813 us x f SDRAM) - 20 =683
  HAL_SDRAM_ProgramRefreshRate(&SDRAM_Handle, 683);
  //  FMC_SetRefreshCount(1386);
}

/**
 * @description: ��ȡ�����ֽ�����
 * @param {void} *pSrcAddress
 * @param {void} *pDstBuffer
 * @param {uint32_t} BufferSize
 * @return {*}
 */
HAL_StatusTypeDef SDRAM_Load(void *pSrcAddress, void *pDstBuffer, uint32_t BufferSize)
{
  uint8_t *d;
  const uint8_t *s;
  d = (uint8_t *)(pDstBuffer);
  s = (uint8_t *)(pSrcAddress);
  while (BufferSize--)
  {
    *d++ = *s++;
  }
  return HAL_OK;
}

/**
 * @description: �洢�����ֽ�����
 * @param {void} *pDirAddress
 * @param {void} *pSrcBuffer
 * @param {uint32_t} BufferSize
 * @return {*}
 */
HAL_StatusTypeDef SDRAM_Store(void *pDirAddress, void *pSrcBuffer, uint32_t BufferSize)
{
  uint8_t *d;
  const uint8_t *s;
  d = (uint8_t *)(pDirAddress);
  s = (uint8_t *)(pSrcBuffer);
  while (BufferSize--)
  {
    *d++ = *s++;
  }
  return HAL_OK;
}

/**
 * @description: ��ȡ���ٸ�8λ����
 * @param {uint32_t} *pSrcAddress
 * @param {uint8_t} *pDstBuffer
 * @param {uint32_t} BufferSize
 * @return {*}
 */
HAL_StatusTypeDef SDRAM_Load_8b(uint32_t *pSrcAddress, uint8_t *pDstBuffer, uint32_t BufferSize)
{
  volatile uint8_t *pSdramAddress = (uint8_t *)pSrcAddress;
  for (uint32_t cnt = 0; cnt < BufferSize; cnt += 1)
  {
    *pDstBuffer = *(volatile uint8_t *)(pSdramAddress + cnt); //�Ӹõ�ַ��������
    pDstBuffer += 1;
  }
  return HAL_OK;
}

/**
 * @description: �洢���ٸ�8λ����
 * @param {uint32_t} *pDirAddress
 * @param {uint8_t} *pSrcBuffer
 * @param {uint32_t} BufferSize
 * @return {*}
 */
HAL_StatusTypeDef SDRAM_Store_8b(uint32_t *pDirAddress, uint8_t *pSrcBuffer, uint32_t BufferSize)
{
  volatile uint8_t *pSdramAddress = (uint8_t *)pDirAddress;
  for (uint32_t cnt = 0; cnt < BufferSize; cnt += 1)
  {
    *(volatile uint8_t *)(pSdramAddress + cnt) = *pSrcBuffer; //�Ӹõ�ַ��������
    pDirAddress += 1;
  }
  return HAL_OK;
}

/**
 * @description: ������Ӧ��ַ������ 8λ
 * @param {uint32_t} *pDirAddress
 * @param {uint8_t} Buffer
 * @param {uint32_t} BufferSize
 * @return {*}
 */
HAL_StatusTypeDef SDRAM_MemSet_8b(uint32_t *pDirAddress, uint8_t Buffer, uint32_t BufferSize)
{
  volatile uint8_t *pSdramAddress = (uint8_t *)pDirAddress;
  for (uint32_t cnt = 0; cnt < BufferSize; cnt += 1)
  {
    *(volatile uint8_t *)(pSdramAddress + cnt) = Buffer; //�Ӹõ�ַ��������
  }
  return HAL_OK;
}

/**
 * @description: ��ȡ���ٸ�16λ����
 * @param {uint32_t} *pSrcAddress
 * @param {uint16_t} *pDstBuffer
 * @param {uint32_t} BufferSize
 * @return {*}
 */
HAL_StatusTypeDef SDRAM_Load_16b(uint32_t *pSrcAddress, uint16_t *pDstBuffer, uint32_t BufferSize)
{
  volatile uint16_t *pSdramAddress = (uint16_t *)pSrcAddress;
  for (uint32_t cnt = 0; cnt < BufferSize / 2; cnt += 1)
  {
    *pDstBuffer = *(volatile uint16_t *)(pSdramAddress + cnt); //�Ӹõ�ַ��������
    pDstBuffer += 1;
  }
  return HAL_OK;
}

/**
 * @description: �洢���ٸ�16λ����
 * @param {uint32_t} *pDirAddress
 * @param {uint16_t} *pSrcBuffer
 * @param {uint32_t} BufferSize
 * @return {*}
 */
HAL_StatusTypeDef SDRAM_Store_16b(uint32_t *pDirAddress, uint16_t *pSrcBuffer, uint32_t BufferSize)
{
  volatile uint16_t *pSdramAddress = (uint16_t *)pDirAddress;
  for (uint32_t cnt = 0; cnt < BufferSize / 2; cnt += 1)
  {
    *(volatile uint16_t *)(pSdramAddress + cnt) = *pSrcBuffer; //�Ӹõ�ַ��������
    pSrcBuffer += 1;
  }
  return HAL_OK;
}

/**
 * @description: ������Ӧ��ַ������ 16λ
 * @param {uint32_t} *pDirAddress
 * @param {uint16_t} Buffer
 * @param {uint32_t} BufferSize
 * @return {*}
 */
HAL_StatusTypeDef SDRAM_MemSet_16b(uint32_t *pDirAddress, uint16_t Buffer, uint32_t BufferSize)
{
  volatile uint16_t *pSdramAddress = (uint16_t *)pDirAddress;
  for (uint32_t cnt = 0; cnt < BufferSize / 2; cnt += 1)
  {
    *(volatile uint16_t *)(pSdramAddress + cnt) = Buffer; //�Ӹõ�ַ��������
  }
  return HAL_OK;
}

/**
 * @description: ��ȡ���ٸ�32λ����
 * @param {uint32_t} *pSrcAddress
 * @param {uint32_t} *pDstBuffer
 * @param {uint32_t} BufferSize
 * @return {*}
 */
HAL_StatusTypeDef SDRAM_Load_32b(uint32_t *pSrcAddress, uint32_t *pDstBuffer, uint32_t BufferSize)
{
  volatile uint32_t *pSdramAddress = (uint32_t *)pSrcAddress;
  for (uint32_t cnt = 0; cnt < BufferSize / 4; cnt += 1)
  {
    *pDstBuffer = *(volatile uint32_t *)(pSdramAddress + cnt); //�Ӹõ�ַ��������
    pDstBuffer += 1;
  }
  return HAL_OK;
}

/**
 * @description: �洢���ٸ�32λ����
 * @param {uint32_t} *pDirAddress
 * @param {uint32_t} *pSrcBuffer
 * @param {uint32_t} BufferSize
 * @return {*}
 */
HAL_StatusTypeDef SDRAM_Store_32b(uint32_t *pDirAddress, uint32_t *pSrcBuffer, uint32_t BufferSize)
{
  volatile uint32_t *pSdramAddress = (uint32_t *)pDirAddress;
  for (uint32_t cnt = 0; cnt < BufferSize / 4; cnt += 1)
  {
    *(volatile uint32_t *)(pSdramAddress + cnt) = *pSrcBuffer; //�Ӹõ�ַ��������
    pSrcBuffer += 1;
  }
  return HAL_OK;
}

/**
 * @description: ������Ӧ��ַ������ 32λ
 * @param {uint32_t} *pDirAddress
 * @param {uint32_t} Buffer
 * @param {uint32_t} BufferSize
 * @return {*}
 */
HAL_StatusTypeDef SDRAM_MemSet_32b(uint32_t *pDirAddress, uint32_t Buffer, uint32_t BufferSize)
{
  volatile uint32_t *pSdramAddress = (uint32_t *)pDirAddress;
  for (uint32_t cnt = 0; cnt < BufferSize / 4; cnt += 1)
  {
    *(volatile uint32_t *)(pSdramAddress + cnt) = Buffer; //�Ӹõ�ַ��������
  }
  return HAL_OK;
}

/**
 * @description: ���ָ����������
 * @param {uint8_t} bank (0-3)
 * @return {*}
 */
HAL_StatusTypeDef SDRAM_EraseBank(uint8_t bank)
{
  if (bank & W9825G6KH_6_BANK1)
  {
    uint32_t startAdd = SDRAM_BANK_ADDR + (0 * W9825G6KH_6_SIZE / W9825G6KH_6_BANK_NUM);
    SDRAM_MemSet_32b((uint32_t *)startAdd, 0x00000000U, W9825G6KH_6_SIZE / W9825G6KH_6_BANK_NUM);
  }
  if (bank & W9825G6KH_6_BANK2)
  {
    uint32_t startAdd = SDRAM_BANK_ADDR + (1 * W9825G6KH_6_SIZE / W9825G6KH_6_BANK_NUM);
    SDRAM_MemSet_32b((uint32_t *)startAdd, 0x00000000U, W9825G6KH_6_SIZE / W9825G6KH_6_BANK_NUM);
  }
  if (bank & W9825G6KH_6_BANK3)
  {
    uint32_t startAdd = SDRAM_BANK_ADDR + (2 * W9825G6KH_6_SIZE / W9825G6KH_6_BANK_NUM);

    SDRAM_MemSet_32b((uint32_t *)startAdd, 0x00000000U, W9825G6KH_6_SIZE / W9825G6KH_6_BANK_NUM);
  }
  if (bank & W9825G6KH_6_BANK4)
  {
    uint32_t startAdd = SDRAM_BANK_ADDR + (3 * W9825G6KH_6_SIZE / W9825G6KH_6_BANK_NUM);
    SDRAM_MemSet_32b((uint32_t *)startAdd, 0x00000000U, W9825G6KH_6_SIZE / W9825G6KH_6_BANK_NUM);
  }

  return HAL_OK;
}

#if SDRAM_TEST
#define DIS_ERR_NUM 25
#define DIS_LINE_NUM 10

void fun_disErrCnt(uint32_t *counter, uint32_t *errCntX, uint32_t *errLineCnt)
{
  ++(*errCntX);
  if (*errCntX < DIS_ERR_NUM)
  {
    printf("0x%x  ", (*counter) | SDRAM_BANK_ADDR);
    ++(*errLineCnt);
    if (*errLineCnt > DIS_LINE_NUM)
    {
      *errLineCnt = 0;
      printf("\r\n ");
    }
  }
  else if (*errCntX == DIS_ERR_NUM)
  {
    *errLineCnt = 0;
    printf("... \r\n");
  }
}
/**
  * @brief  ����SDRAM�Ƿ����� 
  * @param  None
  * @retval ��������1���쳣����0
  */
uint8_t SDRAM_Test8Bit(void)
{
  HAL_StatusTypeDef res;
  uint32_t errLineCnt = 0, errCntA = 0, errCntB = 0, errCntC = 0, errCntD = 0;
  SDRAM_INFO("\r\n���ڼ��SDRAM����8λ�ķ�ʽ��дsdram...\r\n");

#if (SDRAM_CTRL == SDRAM_CTRL_HAL)
  printf("��д����:HAL\r\n");
  /* ��SDRAM����ȫ������Ϊ0 */
  printf("������...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter++)
  {
    uint8_t tmp = 0x00;
    res = HAL_SDRAM_Write_8b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
  }

  /* ������SDRAMд������ */
  printf("д����...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter++)
  {
    uint8_t tmp = (uint8_t)counter;
    res = HAL_SDRAM_Write_8b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
  }

  /* ��ȡ SDRAM ���ݲ����*/
  printf("У����...\r\n");
  printf("8λ���ݶ�д����λ��:\r\n");
  errLineCnt = 0;
  errCntA = 0;
  errCntB = 0;
  errCntC = 0;
  errCntD = 0;
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter++)
  {
    uint8_t tmp = 0x00; //�Ӹõ�ַ��������
    uint8_t tmpB = (uint8_t)(counter & 0xFF);
    HAL_SDRAM_Read_8b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
    if (tmp != tmpB) //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      res = HAL_ERROR;
      if (counter < 0x0800000)
      {
        fun_disErrCnt(&counter, &errCntA, &errLineCnt);
      }
      else if (counter < 0x1000000)
      {
        fun_disErrCnt(&counter, &errCntB, &errLineCnt);
      }
      else if (counter < 0x1800000)
      {
        fun_disErrCnt(&counter, &errCntC, &errLineCnt);
      }
      else if (counter < 0x2000000)
      {
        fun_disErrCnt(&counter, &errCntD, &errLineCnt);
      }
    }
  }
  if (res == HAL_ERROR)
  {
    SDRAM_ERROR("8λ���ݶ�д�������:%d,%d,%d,%d\r\n", errCntA, errCntB, errCntC, errCntD);
  }
  else
  {
    printf("��\r\n");
  }

#else
  printf("\r\n��д����:ָ��\r\n");
  res = HAL_OK;
  /* ��SDRAM����ȫ������Ϊ0 */
  printf("������...\r\n");
  for (uint32_t counter = 0x00; counter < W9825G6KH_6_SIZE; counter++)
  {
    *(volatile uint8_t *)(SDRAM_BANK_ADDR + counter) = (uint8_t)0x00;
  }

  /* ������SDRAMд������ */
  printf("д����...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter++)
  {
    *(volatile uint8_t *)(SDRAM_BANK_ADDR + counter) = (uint8_t)(counter);
  }

  /* ��ȡ SDRAM ���ݲ����*/
  printf("У����...\r\n");
  printf("8λ���ݶ�д����λ��:\r\n");
  errLineCnt = 0;
  errCntA = 0;
  errCntB = 0;
  errCntC = 0;
  errCntD = 0;
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter++)
  {
    uint8_t tmp = *(volatile uint8_t *)(SDRAM_BANK_ADDR + counter); //�Ӹõ�ַ��������
    uint8_t tmpB = (uint8_t)(counter & 0xFF);
    if (tmp != tmpB) //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      res = HAL_ERROR;
      if (counter < 0x0800000)
      {
        fun_disErrCnt(&counter, &errCntA, &errLineCnt);
      }
      else if (counter < 0x1000000)
      {
        fun_disErrCnt(&counter, &errCntB, &errLineCnt);
      }
      else if (counter < 0x1800000)
      {
        fun_disErrCnt(&counter, &errCntC, &errLineCnt);
      }
      else if (counter < 0x2000000)
      {
        fun_disErrCnt(&counter, &errCntD, &errLineCnt);
      }
    }
  }
  if (res == HAL_ERROR)
  {
    SDRAM_ERROR("8λ���ݶ�д�������:%d,%d,%d,%d\r\n", errCntA, errCntB, errCntC, errCntD);
  }
  else
  {
    printf("��\r\n");
  }

#endif

  if (res == HAL_ERROR)
  {
    return 0;
  }
  /*���������return 1 */
  return 1;
}

/**
  * @brief  ����SDRAM�Ƿ����� 
  * @param  None
  * @retval ��������1���쳣����0
  */
uint8_t SDRAM_Test16Bit(void)
{
  HAL_StatusTypeDef res;
  uint32_t errLineCnt = 0, errCntA = 0, errCntB = 0, errCntC = 0, errCntD = 0;
  SDRAM_INFO("\r\n���ڼ��SDRAM����16λ�ķ�ʽ��дsdram...\r\n");

#if (SDRAM_CTRL == SDRAM_CTRL_HAL)
  printf("��д����:HAL\r\n");
  /* ��SDRAM����ȫ������Ϊ0 */
  printf("������...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 2)
  {
    uint16_t tmp = 0x00;
    res = HAL_SDRAM_Write_16b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
  }

  /* ������SDRAMд������ */
  printf("д����...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 2)
  {
    uint16_t tmp = (uint16_t)(counter / 2);
    res = HAL_SDRAM_Write_16b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
  }

  /* ��ȡ SDRAM ���ݲ����*/
  printf("У����...\r\n");
  printf("16λ���ݶ�д����λ��:\r\n");
  errLineCnt = 0;
  errCntA = 0;
  errCntB = 0;
  errCntC = 0;
  errCntD = 0;
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 2)
  {
    uint16_t tmp = 0x00; //�Ӹõ�ַ��������
    uint16_t tmpB = (uint16_t)((counter / 2) & 0xFFFF);
    HAL_SDRAM_Read_16b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
    if (tmp != tmpB) //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      res = HAL_ERROR;
      if (counter < 0x0800000)
      {
        fun_disErrCnt(&counter, &errCntA, &errLineCnt);
      }
      else if (counter < 0x1000000)
      {
        fun_disErrCnt(&counter, &errCntB, &errLineCnt);
      }
      else if (counter < 0x1800000)
      {
        fun_disErrCnt(&counter, &errCntC, &errLineCnt);
      }
      else if (counter < 0x2000000)
      {
        fun_disErrCnt(&counter, &errCntD, &errLineCnt);
      }
    }
  }
  if (res == HAL_ERROR)
  {
    SDRAM_ERROR("16λ���ݶ�д�������:%d,%d,%d,%d\r\n", errCntA, errCntB, errCntC, errCntD);
  }
  else
  {
    printf("��\r\n");
  }

#else
  res = HAL_OK;
  printf("\r\n��д����:ָ��\r\n");
  /* ��SDRAM����ȫ������Ϊ0 */
  printf("������...\r\n");
  for (uint32_t counter = 0x00; counter < W9825G6KH_6_SIZE; counter += 2)
  {
    *(volatile uint16_t *)(SDRAM_BANK_ADDR + counter) = (uint16_t)0x00;
  }

  /* ������SDRAMд������ */
  printf("д����...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 2)
  {
    *(volatile uint16_t *)(SDRAM_BANK_ADDR + counter) = (uint16_t)(counter / 2);
  }

  /* ��ȡ SDRAM ���ݲ����*/
  printf("У����...\r\n");
  printf("16λ���ݶ�д����λ��:\r\n");
  errLineCnt = 0;
  errCntA = 0;
  errCntB = 0;
  errCntC = 0;
  errCntD = 0;
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 2)
  {
    uint16_t tmp = *(volatile uint16_t *)(SDRAM_BANK_ADDR + counter); //�Ӹõ�ַ��������
    uint16_t tmpB = (uint16_t)((counter / 2) & 0xFFFF);
    if (tmp != tmpB) //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      res = HAL_ERROR;
      if (counter < 0x0800000)
      {
        fun_disErrCnt(&counter, &errCntA, &errLineCnt);
      }
      else if (counter < 0x1000000)
      {
        fun_disErrCnt(&counter, &errCntB, &errLineCnt);
      }
      else if (counter < 0x1800000)
      {
        fun_disErrCnt(&counter, &errCntC, &errLineCnt);
      }
      else if (counter < 0x2000000)
      {
        fun_disErrCnt(&counter, &errCntD, &errLineCnt);
      }
    }
  }
  if (res == HAL_ERROR)
  {
    SDRAM_ERROR("16λ���ݶ�д�������:%d,%d,%d,%d\r\n", errCntA, errCntB, errCntC, errCntD);
  }
  else
  {
    printf("��\r\n");
  }

#endif

  if (res == HAL_ERROR)
  {
    return 0;
  }
  /*���������return 1 */
  return 1;
}

/**
  * @brief  ����SDRAM�Ƿ����� 
  * @param  None
  * @retval ��������1���쳣����0
  */
uint8_t SDRAM_Test32Bit(void)
{
  HAL_StatusTypeDef res;
  uint32_t errLineCnt = 0, errCntA = 0, errCntB = 0, errCntC = 0, errCntD = 0;
  SDRAM_INFO("\r\n���ڼ��SDRAM����32λ�ķ�ʽ��дsdram...\r\n");

#if (SDRAM_CTRL == SDRAM_CTRL_HAL)
  printf("��д����:HAL\r\n");
  /* ��SDRAM����ȫ������Ϊ0 */
  printf("������...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 4)
  {
    uint32_t tmp = 0x00000000;
    res = HAL_SDRAM_Write_32b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
  }

  /* ������SDRAMд������ */
  printf("д����...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 4)
  {
    uint32_t tmp = (uint32_t)(counter / 4);
    res = HAL_SDRAM_Write_32b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
  }

  /* ��ȡ SDRAM ���ݲ����*/
  printf("У����...\r\n");
  printf("32λ���ݶ�д����λ��:\r\n");
  errLineCnt = 0;
  errCntA = 0;
  errCntB = 0;
  errCntC = 0;
  errCntD = 0;
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 4)
  {
    uint32_t tmp = 0x00; //�Ӹõ�ַ��������
    uint32_t tmpB = (counter / 4);
    HAL_SDRAM_Read_32b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
    if (tmp != tmpB) //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      res = HAL_ERROR;
      if (counter < 0x0800000)
      {
        fun_disErrCnt(&counter, &errCntA, &errLineCnt);
      }
      else if (counter < 0x1000000)
      {
        fun_disErrCnt(&counter, &errCntB, &errLineCnt);
      }
      else if (counter < 0x1800000)
      {
        fun_disErrCnt(&counter, &errCntC, &errLineCnt);
      }
      else if (counter < 0x2000000)
      {
        fun_disErrCnt(&counter, &errCntD, &errLineCnt);
      }
    }
  }
  if (res == HAL_ERROR)
  {
    SDRAM_ERROR("32λ���ݶ�д�������:%d,%d,%d,%d\r\n", errCntA, errCntB, errCntC, errCntD);
  }
  else
  {
    printf("��\r\n");
  }

#else
  printf("\r\n��д����:ָ��\r\n");
  res = HAL_OK;
  /* ��SDRAM����ȫ������Ϊ0 */
  printf("������...\r\n");
  for (uint32_t counter = 0x00; counter < W9825G6KH_6_SIZE; counter += 4)
  {
    *(volatile uint32_t *)(SDRAM_BANK_ADDR + counter) = (uint32_t)0x00;
  }

  /* ������SDRAMд������ */
  printf("д����...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 4)
  {
    *(volatile uint32_t *)(SDRAM_BANK_ADDR + counter) = (uint32_t)(counter / 4);
  }

  /* ��ȡ SDRAM ���ݲ����*/
  printf("У����...\r\n");
  printf("32λ���ݶ�д����λ��:\r\n");
  errLineCnt = 0;
  errCntA = 0;
  errCntB = 0;
  errCntC = 0;
  errCntD = 0;
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 4)
  {
    uint32_t tmp = *(volatile uint32_t *)(SDRAM_BANK_ADDR + counter); //�Ӹõ�ַ��������
    uint32_t tmpB = (uint32_t)((counter / 4) & 0xFFFFFFFF);
    if (tmp != tmpB) //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      res = HAL_ERROR;
      if (counter < 0x0800000)
      {
        fun_disErrCnt(&counter, &errCntA, &errLineCnt);
      }
      else if (counter < 0x1000000)
      {
        fun_disErrCnt(&counter, &errCntB, &errLineCnt);
      }
      else if (counter < 0x1800000)
      {
        fun_disErrCnt(&counter, &errCntC, &errLineCnt);
      }
      else if (counter < 0x2000000)
      {
        fun_disErrCnt(&counter, &errCntD, &errLineCnt);
      }
    }
  }
  if (res == HAL_ERROR)
  {
    SDRAM_ERROR("32λ���ݶ�д�������:%d,%d,%d,%d\r\n", errCntA, errCntB, errCntC, errCntD);
  }
  else
  {
    printf("��\r\n");
  }

#endif

  if (res == HAL_ERROR)
  {
    return 0;
  }
  /*���������return 1 */
  return 1;
}

void MulTestSDRAM(uint16_t doCnt)
{
  SDRAM_INFO("\r\n<<���ڼ��SDRAM����������%d>>\r\n", doCnt);
  for (uint32_t i = 0; i < doCnt; i++)
  {
    SDRAM_INFO("\r\n<<���ڼ��SDRAM����������%d����ǰ�ִΣ�%d>>\r\n", doCnt, i + 1);
    HAL_Delay(1000);
    if (SDRAM_Test8Bit() == 1)
    {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
      printf("SDRAM��8λ��д���Գɹ�\r\n");
    }
    else
    {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
      printf("SDRAM��8λ��д����ʧ��\r\n");
    }
    printf("\r\n");
    if (SDRAM_Test16Bit() == 1)
    {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
      printf("SDRAM��16λ��д���Գɹ�\r\n");
    }
    else
    {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
      printf("SDRAM��16λ��д����ʧ��\r\n");
    }
    printf("\r\n");
    if (SDRAM_Test32Bit() == 1)
    {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
      printf("SDRAM��32λ��д���Գɹ�\r\n");
    }
    else
    {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
      printf("SDRAM��32λ��д����ʧ��\r\n");
    }
    printf("\r\n");
  }
}

#endif
/*********************************************END OF FILE**********************/
