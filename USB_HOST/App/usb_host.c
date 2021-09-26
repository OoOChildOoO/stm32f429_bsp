/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file            : usb_host.c
  * @version         : v1.0_Cube
  * @brief           : This file implements the USB Host
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_msc.h"

/* USER CODE BEGIN Includes */
#include "usbh_platform.h"
#include "fil_option.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Host core handle declaration */
USBH_HandleTypeDef hUsbHostFS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */
// FATFS mfatfs;
/* USER CODE END 0 */

/*
 * user callback declaration
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * Init USB host library, add supported class and start the library
  * @retval None
  */
void MX_USB_HOST_Init(void)
{
  /* USER CODE BEGIN USB_HOST_Init_PreTreatment */

  /* USER CODE END USB_HOST_Init_PreTreatment */

  /* Init host Library, add supported class and start the library. */
  if (USBH_Init(&hUsbHostFS, USBH_UserProcess, HOST_FS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_RegisterClass(&hUsbHostFS, USBH_MSC_CLASS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_Start(&hUsbHostFS) != USBH_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_HOST_Init_PostTreatment */
  MX_DriverVbusFS(1);
  /* USER CODE END USB_HOST_Init_PostTreatment */
}

/*
 * Background task
 */
void MX_USB_HOST_Process(void)
{
  /* USB Host Background task */
  USBH_Process(&hUsbHostFS);
}
/*
 * user callback definition
 */
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{
  /* USER CODE BEGIN CALL_BACK_1 */
  uint8_t res = 0;
  // uint8_t dataTmp[1024];
  MSC_LUNTypeDef info;
  switch (id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
    printf("HOST_USER_SELECT_CONFIGURATION\r\n");
    break;

  case HOST_USER_DISCONNECTION:
    Appli_state = APPLICATION_DISCONNECT;
    printf("HOST_USER_DISCONNECTION\r\n");
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
    // res = f_mount(NULL, "2:", 1);
    // // res = f_mount(fs[2], "2:", 0);
    // if (res == 0)
    //   printf("usb unmount success \r\n");
    // else
    //   printf("usb unmount fail \r\n");
    break;

  case HOST_USER_CLASS_ACTIVE:

    Appli_state = APPLICATION_READY;
    printf("HOST_USER_CLASS_ACTIVE\r\n");
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

    USBH_MSC_GetLUNInfo(phost, 0, &info);
    res = USBH_MSC_UnitIsReady(phost, 0);
    printf("\r\n USBH_MSC_UnitIsReady： %d\r\n", res);
    res = USBH_MSC_IsReady(phost);
    printf("\r\n USBH_MSC_IsReady: %d\r\n", res);

    printf("\r\n /******************************************/ \r\n");
    printf("\r\n U盘供应商(vendor_id)： %s\r\n", info.inquiry.vendor_id);
    printf("\r\n U盘型号(product_id)： %s\r\n", info.inquiry.product_id);
    printf("\r\n U盘修订版本(revision_id)： %s\r\n", info.inquiry.revision_id);
    printf("\r\n 大小(size)： %d x 1024 ÷ %d = %dGB\r\n", info.capacity.block_nbr,
           info.capacity.block_size, info.capacity.block_nbr / 2 / 1024 / 1024);
    printf("\r\n /******************************************/ \r\n");

    // USBH_MSC_Read(phost, 0, 0x00, dataTmp, 1);
    // printf("\r\n /******************************************/ \r\n");
    // for (uint16_t i = 0; i < 512; i++)
    // {
    //   printf("0x%x ", dataTmp[i]);
    //   if (i % 30 == 0)
    //   {
    //     printf("\r\n");
    //   }
    // }
    // printf("\r\n /******************************************/ \r\n");

    res = f_mount(fs[2], "2:", 1);
    if (res == 0)
    {
      printf("usb mount success \r\n");
    }
    else
    {
      printf("usb mount fail \r\n");
      printf("res = %d \r\n", res);
      break;
    }
    fs_ScanDir("2:");
    break;

  case HOST_USER_CONNECTION:
    Appli_state = APPLICATION_START;
    printf("HOST_USER_CONNECTION\r\n");
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
    break;
  case HOST_USER_CLASS_SELECTED:
    printf("HOST_USER_CLASS_SELECTED\r\n");
    break;

  case HOST_USER_UNRECOVERED_ERROR:
    printf("HOST_USER_UNRECOVERED_ERROR\r\n");
    break;

  default:
    break;
  }
  /* USER CODE END CALL_BACK_1 */
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
