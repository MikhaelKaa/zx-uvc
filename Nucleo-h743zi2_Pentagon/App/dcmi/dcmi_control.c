
#include "main.h"
#include <stdio.h>
#include "dcmi.h"

extern DCMI_HandleTypeDef hdcmi;
extern /*__attribute__ ((section(".RAM_D2_buf"), used))*/ uint8_t zx_buf[2][ZX_V][ZX_H];

void dcmi_start(void) {
    int dcmi_ret = HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)zx_buf, sizeof(zx_buf)/4);
    printf("HAL_DCMI_Start_DMA return %d\r\n", dcmi_ret);
}

void dcmi_stop(void) {  
    int dcmi_ret = HAL_DCMI_Stop(&hdcmi);
    printf("HAL_DCMI_Stop return %d\r\n", dcmi_ret);
}

void dcmi_suspend(void) {  
    int dcmi_ret = HAL_DCMI_Suspend(&hdcmi);
    printf("HAL_DCMI_Suspend return %d\r\n", dcmi_ret);
}

void dcmi_resume(void) {  
    int dcmi_ret = HAL_DCMI_Resume(&hdcmi);
    printf("HAL_DCMI_Resume return %d\r\n", dcmi_ret);
}

void dcmi_toogle_VS_polarity(void) {
    //0: DCMI_VSYNC active low
    //1: DCMI_VSYNC active high
    hdcmi.Instance->CR ^=  DCMI_CR_VSPOL;
    printf("DCMI Vertical sync polarity is %ld\r\n", (hdcmi.Instance->CR>>DCMI_CR_VSPOL_Pos) & 1U);
}

void dcmi_toogle_HS_polarity(void) {
    //0: DCMI_HSYNC active low
    //1: DCMI_HSYNC active high
    hdcmi.Instance->CR ^=  DCMI_CR_HSPOL;
    printf("DCMI Horizontal sync polarity is %ld\r\n", (hdcmi.Instance->CR>>DCMI_CR_HSPOL_Pos) & 1U);
}

void dcmi_toogle_PIXCLK_edge(void) {
    //0: Falling edge active
    //1: Rising edge active
    hdcmi.Instance->CR ^=  DCMI_CR_PCKPOL;
    //printf("DCMI pixel clock edge is %ld\r\n", (hdcmi.Instance->CR>>DCMI_CR_PCKPOL_Pos)&1);
    if((hdcmi.Instance->CR>>DCMI_CR_PCKPOL_Pos) & 1U)
        printf("1: Rising edge active\r\n");
    else
        printf("0: Falling edge active\r\n");
}


void case_help(void) {
    printf("\r\n\
        help message\r\n\
        z - start DCMI\r\n\
        x - stop DCMI\r\n\
        c - suspend DCMI\r\n\
        v - resume DCMI\r\n\
        b - toggle horizontal sync polaryty\r\n\
        n - toggle vertical sync polaryty\r\n\
        m - toggle pixel clock edge\r\n\
        ");
}

void dcmi_control(uint8_t cmd) {
    switch (cmd) {
    case 'h': case_help(); break;
    case 'z': dcmi_start(); break;
    case 'x': dcmi_stop(); break;
    case 'c': dcmi_suspend(); break;
    case 'v': dcmi_resume(); break;
    case 'b': dcmi_toogle_HS_polarity(); break;
    case 'n': dcmi_toogle_VS_polarity(); break;
    case 'm': dcmi_toogle_PIXCLK_edge(); break;
    
    default: break;
  }
}
