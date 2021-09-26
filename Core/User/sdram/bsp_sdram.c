/**
  ******************************************************************************
  * @file    bsp_sdram.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   sdram应用函数接口
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

  /* FMC SDRAM 控制配置 */
  /* SDBank:选择存储区域*/
  /* ColumnBitsNumber:行地址线宽度: [8:0] */
  /* RowBitsNumber:列地址线宽度: [12:0] */
  /* MemoryDataWidth:数据线宽度 */
  /* InternalBankNumber:SDRAM内部bank数量*/
  /* CASLatency:CAS潜伏期 */
  /* WriteProtection:禁止写保护*/
  /* SDClockPeriod:SDCLK时钟分频因子，SDCLK = HCLK/SDCLOCK_PERIOD*/
  /* ReadBurst:突发读模式设置*/
  /* ReadPipeDelay:读延迟配置 */

  /* SDRAM时序参数 */
  /* SDRAM时序结构体，根据SDRAM参数表配置----------------*/
  /* fSDCLK： 90 Mhz (HCLK/2 :180Mhz/2) 1个时钟周期tSDCLK =1/90MHz=11.11ns*/
  /* tMRD(RSC): min=2 *tSDCLK   LoadToActiveDelay:加载模式寄存器命令和行有效或刷新命令之间的延迟*/
  /* tXSR: min=72ns (7x11.11ns)   ExitSelfRefreshDelay:退出自我刷新到行有效命令之间的延迟*/
  /* tRAS: min=42ns (4x11.11ns) max=100000 (ns)   SelfRefreshTime:行有效与预充电命令之间的延迟*/
  /* tRC:  min=60ns (6x11.11ns)   RowCycleDelay:两个刷新命令或者两个行有效命令之间的延迟*/
  /* tWR:  min=2 *tSDCLK    WriteRecoveryTime:写入命令到预充电命令之间的延迟*/
  /* tRP:  min=15ns (2x11.11ns)   RPDelay:预充电与行有效命令之间的延迟*/
  /* tRCD: min=15ns (2x11.11ns)   RCDDelay:行有效与列读写命令之间的延迟*/

  SDRAM_InitSequence();
  SDRAM_EraseBank(W9825G6KH_6_BANK1 | W9825G6KH_6_BANK2 | W9825G6KH_6_BANK3 | W9825G6KH_6_BANK4);
  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief  对SDRAM芯片进行初始化配置
  * @param  None. 
  * @retval None.
  */
void SDRAM_InitSequence(void)
{
  FMC_SDRAM_CommandTypeDef Command;
  uint32_t tmpr = 0;
  /* Step 1  配置命令：开启提供给SDRAM的时钟*/
  Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;

  HAL_SDRAM_SendCommand(&SDRAM_Handle, &Command, SDRAM_TIMEOUT);

  /* Step 2  配置命令：延迟至少 200us */
  //SDRAM_delay(1);
  HAL_Delay(1);

  /* Step 3  配置命令：对所有的bank预充电 */
  Command.CommandMode = FMC_SDRAM_CMD_PALL;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;

  HAL_SDRAM_SendCommand(&SDRAM_Handle, &Command, SDRAM_TIMEOUT);

  /* Step 4  配置命令：插入8个自动刷新周期 */
  Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 8;
  Command.ModeRegisterDefinition = 0;

  HAL_SDRAM_SendCommand(&SDRAM_Handle, &Command, SDRAM_TIMEOUT);

  /* Step 5  配置命令：配置SRAM的加载模式寄存器 */
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

  /* Step 6  配置命令：配置FMC的SDRAM控制器的自动刷新周期 */
  //SDRAM refresh rate = T sdram refresh period  / number of rows  =  64ms/8192 = 7.813us
  //count = (SDRAM refresh rate * f SDRAM) - 20 = (7.813 us x f SDRAM) - 20 =683
  HAL_SDRAM_ProgramRefreshRate(&SDRAM_Handle, 683);
  //  FMC_SetRefreshCount(1386);
}

/**
 * @description: 读取多少字节数据
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
 * @description: 存储多少字节数据
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
 * @description: 读取多少个8位数据
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
    *pDstBuffer = *(volatile uint8_t *)(pSdramAddress + cnt); //从该地址读出数据
    pDstBuffer += 1;
  }
  return HAL_OK;
}

/**
 * @description: 存储多少个8位数据
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
    *(volatile uint8_t *)(pSdramAddress + cnt) = *pSrcBuffer; //从该地址读出数据
    pDirAddress += 1;
  }
  return HAL_OK;
}

/**
 * @description: 设置相应地址的内容 8位
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
    *(volatile uint8_t *)(pSdramAddress + cnt) = Buffer; //从该地址读出数据
  }
  return HAL_OK;
}

/**
 * @description: 读取多少个16位数据
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
    *pDstBuffer = *(volatile uint16_t *)(pSdramAddress + cnt); //从该地址读出数据
    pDstBuffer += 1;
  }
  return HAL_OK;
}

/**
 * @description: 存储多少个16位数据
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
    *(volatile uint16_t *)(pSdramAddress + cnt) = *pSrcBuffer; //从该地址读出数据
    pSrcBuffer += 1;
  }
  return HAL_OK;
}

/**
 * @description: 设置相应地址的内容 16位
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
    *(volatile uint16_t *)(pSdramAddress + cnt) = Buffer; //从该地址读出数据
  }
  return HAL_OK;
}

/**
 * @description: 读取多少个32位数据
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
    *pDstBuffer = *(volatile uint32_t *)(pSdramAddress + cnt); //从该地址读出数据
    pDstBuffer += 1;
  }
  return HAL_OK;
}

/**
 * @description: 存储多少个32位数据
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
    *(volatile uint32_t *)(pSdramAddress + cnt) = *pSrcBuffer; //从该地址读出数据
    pSrcBuffer += 1;
  }
  return HAL_OK;
}

/**
 * @description: 设置相应地址的内容 32位
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
    *(volatile uint32_t *)(pSdramAddress + cnt) = Buffer; //从该地址读出数据
  }
  return HAL_OK;
}

/**
 * @description: 清除指定扇区数据
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
  * @brief  测试SDRAM是否正常 
  * @param  None
  * @retval 正常返回1，异常返回0
  */
uint8_t SDRAM_Test8Bit(void)
{
  HAL_StatusTypeDef res;
  uint32_t errLineCnt = 0, errCntA = 0, errCntB = 0, errCntC = 0, errCntD = 0;
  SDRAM_INFO("\r\n正在检测SDRAM，以8位的方式读写sdram...\r\n");

#if (SDRAM_CTRL == SDRAM_CTRL_HAL)
  printf("读写方法:HAL\r\n");
  /* 把SDRAM数据全部重置为0 */
  printf("擦除中...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter++)
  {
    uint8_t tmp = 0x00;
    res = HAL_SDRAM_Write_8b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
  }

  /* 向整个SDRAM写入数据 */
  printf("写入中...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter++)
  {
    uint8_t tmp = (uint8_t)counter;
    res = HAL_SDRAM_Write_8b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
  }

  /* 读取 SDRAM 数据并检测*/
  printf("校验中...\r\n");
  printf("8位数据读写错误位号:\r\n");
  errLineCnt = 0;
  errCntA = 0;
  errCntB = 0;
  errCntC = 0;
  errCntD = 0;
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter++)
  {
    uint8_t tmp = 0x00; //从该地址读出数据
    uint8_t tmpB = (uint8_t)(counter & 0xFF);
    HAL_SDRAM_Read_8b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
    if (tmp != tmpB) //检测数据，若不相等，跳出函数,返回检测失败结果。
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
    SDRAM_ERROR("8位数据读写错误个数:%d,%d,%d,%d\r\n", errCntA, errCntB, errCntC, errCntD);
  }
  else
  {
    printf("无\r\n");
  }

#else
  printf("\r\n读写方法:指针\r\n");
  res = HAL_OK;
  /* 把SDRAM数据全部重置为0 */
  printf("擦除中...\r\n");
  for (uint32_t counter = 0x00; counter < W9825G6KH_6_SIZE; counter++)
  {
    *(volatile uint8_t *)(SDRAM_BANK_ADDR + counter) = (uint8_t)0x00;
  }

  /* 向整个SDRAM写入数据 */
  printf("写入中...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter++)
  {
    *(volatile uint8_t *)(SDRAM_BANK_ADDR + counter) = (uint8_t)(counter);
  }

  /* 读取 SDRAM 数据并检测*/
  printf("校验中...\r\n");
  printf("8位数据读写错误位号:\r\n");
  errLineCnt = 0;
  errCntA = 0;
  errCntB = 0;
  errCntC = 0;
  errCntD = 0;
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter++)
  {
    uint8_t tmp = *(volatile uint8_t *)(SDRAM_BANK_ADDR + counter); //从该地址读出数据
    uint8_t tmpB = (uint8_t)(counter & 0xFF);
    if (tmp != tmpB) //检测数据，若不相等，跳出函数,返回检测失败结果。
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
    SDRAM_ERROR("8位数据读写错误个数:%d,%d,%d,%d\r\n", errCntA, errCntB, errCntC, errCntD);
  }
  else
  {
    printf("无\r\n");
  }

#endif

  if (res == HAL_ERROR)
  {
    return 0;
  }
  /*检测正常，return 1 */
  return 1;
}

/**
  * @brief  测试SDRAM是否正常 
  * @param  None
  * @retval 正常返回1，异常返回0
  */
uint8_t SDRAM_Test16Bit(void)
{
  HAL_StatusTypeDef res;
  uint32_t errLineCnt = 0, errCntA = 0, errCntB = 0, errCntC = 0, errCntD = 0;
  SDRAM_INFO("\r\n正在检测SDRAM，以16位的方式读写sdram...\r\n");

#if (SDRAM_CTRL == SDRAM_CTRL_HAL)
  printf("读写方法:HAL\r\n");
  /* 把SDRAM数据全部重置为0 */
  printf("擦除中...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 2)
  {
    uint16_t tmp = 0x00;
    res = HAL_SDRAM_Write_16b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
  }

  /* 向整个SDRAM写入数据 */
  printf("写入中...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 2)
  {
    uint16_t tmp = (uint16_t)(counter / 2);
    res = HAL_SDRAM_Write_16b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
  }

  /* 读取 SDRAM 数据并检测*/
  printf("校验中...\r\n");
  printf("16位数据读写错误位号:\r\n");
  errLineCnt = 0;
  errCntA = 0;
  errCntB = 0;
  errCntC = 0;
  errCntD = 0;
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 2)
  {
    uint16_t tmp = 0x00; //从该地址读出数据
    uint16_t tmpB = (uint16_t)((counter / 2) & 0xFFFF);
    HAL_SDRAM_Read_16b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
    if (tmp != tmpB) //检测数据，若不相等，跳出函数,返回检测失败结果。
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
    SDRAM_ERROR("16位数据读写错误个数:%d,%d,%d,%d\r\n", errCntA, errCntB, errCntC, errCntD);
  }
  else
  {
    printf("无\r\n");
  }

#else
  res = HAL_OK;
  printf("\r\n读写方法:指针\r\n");
  /* 把SDRAM数据全部重置为0 */
  printf("擦除中...\r\n");
  for (uint32_t counter = 0x00; counter < W9825G6KH_6_SIZE; counter += 2)
  {
    *(volatile uint16_t *)(SDRAM_BANK_ADDR + counter) = (uint16_t)0x00;
  }

  /* 向整个SDRAM写入数据 */
  printf("写入中...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 2)
  {
    *(volatile uint16_t *)(SDRAM_BANK_ADDR + counter) = (uint16_t)(counter / 2);
  }

  /* 读取 SDRAM 数据并检测*/
  printf("校验中...\r\n");
  printf("16位数据读写错误位号:\r\n");
  errLineCnt = 0;
  errCntA = 0;
  errCntB = 0;
  errCntC = 0;
  errCntD = 0;
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 2)
  {
    uint16_t tmp = *(volatile uint16_t *)(SDRAM_BANK_ADDR + counter); //从该地址读出数据
    uint16_t tmpB = (uint16_t)((counter / 2) & 0xFFFF);
    if (tmp != tmpB) //检测数据，若不相等，跳出函数,返回检测失败结果。
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
    SDRAM_ERROR("16位数据读写错误个数:%d,%d,%d,%d\r\n", errCntA, errCntB, errCntC, errCntD);
  }
  else
  {
    printf("无\r\n");
  }

#endif

  if (res == HAL_ERROR)
  {
    return 0;
  }
  /*检测正常，return 1 */
  return 1;
}

/**
  * @brief  测试SDRAM是否正常 
  * @param  None
  * @retval 正常返回1，异常返回0
  */
uint8_t SDRAM_Test32Bit(void)
{
  HAL_StatusTypeDef res;
  uint32_t errLineCnt = 0, errCntA = 0, errCntB = 0, errCntC = 0, errCntD = 0;
  SDRAM_INFO("\r\n正在检测SDRAM，以32位的方式读写sdram...\r\n");

#if (SDRAM_CTRL == SDRAM_CTRL_HAL)
  printf("读写方法:HAL\r\n");
  /* 把SDRAM数据全部重置为0 */
  printf("擦除中...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 4)
  {
    uint32_t tmp = 0x00000000;
    res = HAL_SDRAM_Write_32b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
  }

  /* 向整个SDRAM写入数据 */
  printf("写入中...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 4)
  {
    uint32_t tmp = (uint32_t)(counter / 4);
    res = HAL_SDRAM_Write_32b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
  }

  /* 读取 SDRAM 数据并检测*/
  printf("校验中...\r\n");
  printf("32位数据读写错误位号:\r\n");
  errLineCnt = 0;
  errCntA = 0;
  errCntB = 0;
  errCntC = 0;
  errCntD = 0;
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 4)
  {
    uint32_t tmp = 0x00; //从该地址读出数据
    uint32_t tmpB = (counter / 4);
    HAL_SDRAM_Read_32b(&hsdram1, (uint32_t *)(SDRAM_BANK_ADDR + counter), &tmp, 1);
    if (tmp != tmpB) //检测数据，若不相等，跳出函数,返回检测失败结果。
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
    SDRAM_ERROR("32位数据读写错误个数:%d,%d,%d,%d\r\n", errCntA, errCntB, errCntC, errCntD);
  }
  else
  {
    printf("无\r\n");
  }

#else
  printf("\r\n读写方法:指针\r\n");
  res = HAL_OK;
  /* 把SDRAM数据全部重置为0 */
  printf("擦除中...\r\n");
  for (uint32_t counter = 0x00; counter < W9825G6KH_6_SIZE; counter += 4)
  {
    *(volatile uint32_t *)(SDRAM_BANK_ADDR + counter) = (uint32_t)0x00;
  }

  /* 向整个SDRAM写入数据 */
  printf("写入中...\r\n");
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 4)
  {
    *(volatile uint32_t *)(SDRAM_BANK_ADDR + counter) = (uint32_t)(counter / 4);
  }

  /* 读取 SDRAM 数据并检测*/
  printf("校验中...\r\n");
  printf("32位数据读写错误位号:\r\n");
  errLineCnt = 0;
  errCntA = 0;
  errCntB = 0;
  errCntC = 0;
  errCntD = 0;
  for (uint32_t counter = 0; counter < W9825G6KH_6_SIZE; counter += 4)
  {
    uint32_t tmp = *(volatile uint32_t *)(SDRAM_BANK_ADDR + counter); //从该地址读出数据
    uint32_t tmpB = (uint32_t)((counter / 4) & 0xFFFFFFFF);
    if (tmp != tmpB) //检测数据，若不相等，跳出函数,返回检测失败结果。
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
    SDRAM_ERROR("32位数据读写错误个数:%d,%d,%d,%d\r\n", errCntA, errCntB, errCntC, errCntD);
  }
  else
  {
    printf("无\r\n");
  }

#endif

  if (res == HAL_ERROR)
  {
    return 0;
  }
  /*检测正常，return 1 */
  return 1;
}

void MulTestSDRAM(uint16_t doCnt)
{
  SDRAM_INFO("\r\n<<正在检测SDRAM，总轮数：%d>>\r\n", doCnt);
  for (uint32_t i = 0; i < doCnt; i++)
  {
    SDRAM_INFO("\r\n<<正在检测SDRAM，总轮数：%d，当前轮次：%d>>\r\n", doCnt, i + 1);
    HAL_Delay(1000);
    if (SDRAM_Test8Bit() == 1)
    {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
      printf("SDRAM的8位读写测试成功\r\n");
    }
    else
    {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
      printf("SDRAM的8位读写测试失败\r\n");
    }
    printf("\r\n");
    if (SDRAM_Test16Bit() == 1)
    {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
      printf("SDRAM的16位读写测试成功\r\n");
    }
    else
    {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
      printf("SDRAM的16位读写测试失败\r\n");
    }
    printf("\r\n");
    if (SDRAM_Test32Bit() == 1)
    {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
      printf("SDRAM的32位读写测试成功\r\n");
    }
    else
    {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
      printf("SDRAM的32位读写测试失败\r\n");
    }
    printf("\r\n");
  }
}

#endif
/*********************************************END OF FILE**********************/
