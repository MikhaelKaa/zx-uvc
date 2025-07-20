#include "main.h"
#include <stdio.h>
#include "dcmi.h"

void (*copy_pixels)(void) = zx_copy_pix_pent;

// gmx-scorpion 296x432
// gmx-pentagon 304x432
#pragma pack(push, 1)
//RAM_D2 uint8_t zx_buf_gmx_sc[296][432];
//RAM_D2 uint8_t zx_buf_gmx_pent[304][432];
RAM_D2 uint8_t zx_buf_pent[304][432-16];
#pragma pack(pop)

// Таблица соответствия пикселся спектрума (RGBI) пикселю RGB565.
__attribute__ ((section(".DTCMRAM_buf"), used)) uint16_t zx_pix_tab[16];

void init_pix_table(void)
{
  uint8_t  pix_i;
  uint16_t pix_r;
  uint16_t pix_g;
  uint16_t pix_b;
  uint8_t palette_R = 15;
  uint8_t palette_G = 30;
  uint8_t palette_B = 15;
  uint8_t palette_IR = 15;
  uint8_t palette_IG = 30;
  uint8_t palette_IB = 15;

  for(int n = 0; n < 16; n++) {
    pix_i = (n & 0b00001000)?(1U):(0);
    pix_r = (n & 0b00000001)?(palette_R + palette_IR * pix_i):(0);
    pix_g = (n & 0b00000010)?(palette_G + palette_IG * pix_i):(0);
    pix_b = (n & 0b00000100)?(palette_B + palette_IB * pix_i):(0);
    zx_pix_tab[n] = (pix_b<<0) | (pix_g<<5) | (pix_r<<11);
  }
}

// time ~7.7ms @480MHz (stm32h743, gcc -O3)
void zx_copy_pix_gmx_sc(void)
{
  for(int j = 0; j < UVC_VIDEO_HEIGHT; j++) {
    for(int k = 0; k < UVC_VIDEO_WIDTH; k++) {
      ucv_buf[uvc_cnt%2][(UVC_VIDEO_HEIGHT-1)-j][k] = zx_pix_tab[zx_buf_pent[j+42][k+88]];
    }
  }
}

// time ~3.9ms @480MHz (stm32h743, gcc -O3)
void zx_copy_pix_gmx_pent(void)
{
  for(int j = 0; j < UVC_VIDEO_HEIGHT; j++) {
    for(int k = 0; k < UVC_VIDEO_WIDTH; k++) {
      ucv_buf[uvc_cnt%2][(UVC_VIDEO_HEIGHT-1)-j][k] = zx_pix_tab[zx_buf_pent[j+42][k+88]];
    }
  }
}

void zx_copy_pix_pent(void)
{
  for(int j = 0; j < UVC_VIDEO_HEIGHT; j++) {
    for(int k = 0; k < UVC_VIDEO_WIDTH; k++) {
      ucv_buf[uvc_cnt%2][(UVC_VIDEO_HEIGHT-1)-j][k] = zx_pix_tab[zx_buf_pent[j+42][k+72]];
    }
  }
}


int ZX_CAP_Proc(void) {
    if(DCMI_flag) {
      HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
      copy_pixels();
      HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
      UVC_flag = 1;
      DCMI_flag = 0;
      return 0;
    }
    return -1;
}