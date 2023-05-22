/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dcmi.h"
#include "dma.h"
#include "rng.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "retarget.h"
#include <string.h>
#include "dcmi_control.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
void (*copy_pixels)(void) = zx_copy_pix_gmx_sc;


#ifdef UVC_USE_RGB565
RAM_D1 uint16_t ucv_buf[UVC_VIDEO_HEIGHT][UVC_VIDEO_WIDTH];
#endif

// gmx-scorpion 296x432
// gmx-pentagon 304x432
#pragma pack(push, 1)
RAM_D2 uint8_t zx_buf_gmx_sc[296][432];
RAM_D2 uint8_t zx_buf_gmx_pent[304][432];
#pragma pack(pop)
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi) {
}

void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi) {

}

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi) {
  DCMI_flag = 1;
  HAL_GPIO_TogglePin(test_pin0_GPIO_Port, test_pin0_Pin);
  UVC_flag = 0;
  
}
void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi) {
  //HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

__attribute__ ((section(".DTCMRAM_buf"), used)) uint16_t zx_pix_tab[16];
uint8_t  pix_i;
uint16_t pix_r;
uint16_t pix_g;
uint16_t pix_b;
uint8_t pal_r = 15;
uint8_t pal_g = 30;
uint8_t pal_b = 15;
uint8_t pal_ir = 15;
uint8_t pal_ig = 30;
uint8_t pal_ib = 15;

void init_pix_table(void)
{
  for(int n = 0; n < 16; n++) {
    pix_i = (n & 0b00001000)?(1U):(0);
    pix_r = (n & 0b00000001)?(pal_r + pal_ir * pix_i):(0);
    pix_g = (n & 0b00000010)?(pal_g + pal_ig * pix_i):(0);
    pix_b = (n & 0b00000100)?(pal_b + pal_ib * pix_i):(0);
    zx_pix_tab[n] = (pix_b<<0) | (pix_g<<5) | (pix_r<<11);
  }
}

void zx_copy_pix_gmx_sc(void)
{
  for(int j = 0; j < UVC_VIDEO_HEIGHT; j++) {
    for(int k = 0; k < UVC_VIDEO_WIDTH; k++) {
      ucv_buf[(UVC_VIDEO_HEIGHT-1)-j][k] = zx_pix_tab[zx_buf_gmx_sc[j+42][k+88]];
    }
  }
}

void zx_copy_pix_gmx_pent(void)
{
  for(int j = 0; j < UVC_VIDEO_HEIGHT; j++) {
    for(int k = 0; k < UVC_VIDEO_WIDTH; k++) {
      ucv_buf[(UVC_VIDEO_HEIGHT-1)-j][k] = zx_pix_tab[zx_buf_gmx_pent[j+42][k+88]];
    }
  }
}

RAM_D1 uint8_t rx[2] = {0, 0};
void (*uart_rx_callback)(uint8_t) = dcmi_control;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART3) {
    uart_rx_callback(rx[1]);
  }   
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART3) {
    uart_rx_callback(rx[0]);
  }   
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

    offset_x = 40;
    offset_y = 72;
  
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USB_DEVICE_Init();
  MX_RNG_Init();
  MX_DCMI_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */


  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
  HAL_GPIO_WritePin(ULPI_RESET_GPIO_Port, ULPI_RESET_Pin, GPIO_PIN_SET);
  HAL_Delay(10);
  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
  HAL_GPIO_WritePin(ULPI_RESET_GPIO_Port, ULPI_RESET_Pin, GPIO_PIN_RESET);
  HAL_UART_Receive_DMA(&huart3, &rx[0], sizeof(rx));
  memset(zx_buf_gmx_pent, 0x55, sizeof(zx_buf_gmx_pent));
  memset(zx_buf_gmx_sc, 0x55, sizeof(zx_buf_gmx_sc));
  memset(ucv_buf, 0x55, sizeof(ucv_buf));
  init_pix_table();
  printf("zx uvc start\r\n");

    //
    // ZX_CAPTURE - library
    //
    ZX_CAPTURE_IfTypeDef        _zx_interface;
    ZX_CAPTURE_HandleTypDef     _zx_handle;

    _zx_interface.ZXCAPTURE_IF_DelayMs          = func;
    _zx_interface.ZXCAPTURE_IF_GPIO_ResetPin    = func;
    _zx_interface.ZXCAPTURE_IF_WritePin         = func;
    _zx_interface.ZXCAPTURE_IF_GetTickMs        = func;
    _zx_interface.ZXCAPTURE_IF_Receive          = func;
    _zx_interface.ZXCAPTURE_IF_Transmit         = func;

    ZX_CAPTURE_init(&_zx_handle, _zx_interface,1) ;
    //
    //
    //
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if(DCMI_flag) {
      copy_pixels();
      UVC_flag = 1;
      DCMI_flag = 0;
    }
    printf_flush();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 15;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO2, RCC_MCO2SOURCE_HSE, RCC_MCODIV_1);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

