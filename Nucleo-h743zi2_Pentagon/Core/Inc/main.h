/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define RAM_D1 __attribute__ ((section(".RAM_D1_buf"), used)) 
#define RAM_D2 __attribute__ ((section(".RAM_D2_buf"), used)) 
#define RAM_D3 __attribute__ ((section(".RAM_D3_buf"), used)) 
//#define UVC_USE_RGB888
#define UVC_USE_RGB565
//#define UVC_USE_RGB555

//int zx_h = (448U);
//int zx_v = (320U);

void zx_copy_pix(void);
void zx_copy_pix_test(void);
uint8_t UVC_flag;
int offset_x, offset_y;
void (*copy_pixels)(void);

// Ширина экрана пентагона полная.
#define PENT_H (448U)
// Высота экрана пентагона полная.
#define PENT_V (320U)
// Кадровый импульс экрана пентагона.
//#define PENT_HS (32U)
#define PENT_HS (32U-16U)
// Строчный импульс экрана пентагона.
#define PENT_VS (16U+8U)
//#define PENT_VS (16U)

// Обрежем кадр до 320x240
#define PENT_HCrop (96U)
#define PENT_VCrop (64U)


#define ZX_H (PENT_H-PENT_HS)
#define ZX_V (PENT_V-PENT_VS)

#define ZX_BORDER 1

#if ZX_BORDER == 1
//#define ZX_PIX(x, y) (zx_buf[x][y+16])
#define ZX_PIX(x, y) (zx_buf[(p+1)%2][x+40][y+72])
//#define UVC_VIDEO_WIDTH        (ZX_H-PENT_HCrop)
#define UVC_VIDEO_WIDTH        (320U)
//#define UVC_VIDEO_HEIGHT      (ZX_V-PENT_VCrop)
#define UVC_VIDEO_HEIGHT      (240U) 
#endif

#if ZX_BORDER == 0
//#define ZX_PIX(x, y) (zx_buf[p][x+64][y+104])
#define ZX_PIX(x, y) (zx_buf[p][x][y])
#define UVC_VIDEO_WIDTH        256
#define UVC_VIDEO_HEIGHT       192
#endif

#ifdef UVC_USE_RGB555
#define UVC_BYTE_PER_PIX (2U)
extern uint16_t ucv_buf[UVC_VIDEO_HEIGHT][UVC_VIDEO_WIDTH];
#endif

#ifdef UVC_USE_RGB565
#define UVC_BYTE_PER_PIX (2U)
extern uint16_t ucv_buf[UVC_VIDEO_HEIGHT][UVC_VIDEO_WIDTH];
#endif

#ifdef UVC_USE_RGB888
#define UVC_BYTE_PER_PIX (3U)
typedef struct {
  uint8_t b;
  uint8_t g;
  uint8_t r;
} pixel888_t;
extern pixel888_t ucv_buf[UVC_VIDEO_HEIGHT][UVC_VIDEO_WIDTH];
#endif

//extern uint8_t zx_buf[2][ZX_V][ZX_H];
uint8_t uvc_wait_flag;
extern int uvc_wait_cnt;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define STLINK_RX_Pin GPIO_PIN_8
#define STLINK_RX_GPIO_Port GPIOD
#define STLINK_TX_Pin GPIO_PIN_9
#define STLINK_TX_GPIO_Port GPIOD
#define USB_OTG_FS_PWR_EN_Pin GPIO_PIN_10
#define USB_OTG_FS_PWR_EN_GPIO_Port GPIOD
#define ULPI_RESET_Pin GPIO_PIN_13
#define ULPI_RESET_GPIO_Port GPIOD
#define USB_OTG_FS_OVCR_Pin GPIO_PIN_7
#define USB_OTG_FS_OVCR_GPIO_Port GPIOG
#define test_pin0_Pin GPIO_PIN_15
#define test_pin0_GPIO_Port GPIOA
#define test_pin1_Pin GPIO_PIN_10
#define test_pin1_GPIO_Port GPIOC
#define test_pin2_Pin GPIO_PIN_12
#define test_pin2_GPIO_Port GPIOC
#define LD2_Pin GPIO_PIN_1
#define LD2_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
