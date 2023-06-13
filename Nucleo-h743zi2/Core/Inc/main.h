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

#define UVC_USE_RGB565



//extern uint8_t zx_buf_gmx_sc[296][432];
//extern uint8_t zx_buf_gmx_pent[304][432];
//extern uint8_t zx_buf_pent[304][432-16];
extern uint8_t dcmi_buf[131328]; 
#define ZX_GMX_SCORPION_LEN (127872U)
#define ZX_GMX_PENTAGON_LEN (131328U)
extern uint16_t pix_per_line;

void zx_copy_pix(void);
void zx_copy_pix_gmx_sc(void);
void zx_copy_pix_gmx_pent(void);
void zx_copy_pix_pent(void);

extern uint8_t UVC_flag;
extern int offset_x;
extern int  offset_y;
extern void (*copy_pixels)(void);

#ifdef UVC_USE_RGB565
#define UVC_VIDEO_WIDTH        (320U)
//#define UVC_VIDEO_HEIGHT      (ZX_V-PENT_VCrop)
#define UVC_VIDEO_HEIGHT      (240U) 
#define UVC_BYTE_PER_PIX (2U)
extern uint16_t ucv_buf[UVC_VIDEO_HEIGHT][UVC_VIDEO_WIDTH];
#endif

extern uint8_t DCMI_flag;

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
