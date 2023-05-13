
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

void ptint_vs_pol(void){
    if((hdcmi.Instance->CR>>DCMI_CR_VSPOL_Pos) & 1U){
        //1: DCMI_VSYNC active high
        printf("DCMI_VSYNC active high\r\n");
    } else {
        //0: DCMI_VSYNC active low
        printf("DCMI_VSYNC active low\r\n");
    }
}

void print_hs_pol(void) {
    if((hdcmi.Instance->CR>>DCMI_CR_HSPOL_Pos) & 1U){
        //1: DCMI_HSYNC active high
        printf("DCMI_HSYNC active high\r\n");
    } else {
        //0: DCMI_HSYNC active low
        printf("DCMI_HSYNC active low\r\n");
    }
}

void print_pixclk(void) {
    if((hdcmi.Instance->CR>>DCMI_CR_PCKPOL_Pos) & 1U)
        //1: Rising edge active
        printf("DCMI pixel clock Rising edge active\r\n");
    else
        //0: Falling edge active
        printf("DCMI pixel clock Falling edge active\r\n");
}

void dcmi_toogle_VS_polarity(void) {
    hdcmi.Instance->CR ^=  DCMI_CR_VSPOL;
    ptint_vs_pol();
}

void dcmi_toogle_HS_polarity(void) {
    hdcmi.Instance->CR ^=  DCMI_CR_HSPOL;
    print_hs_pol();
}

void dcmi_toogle_PIXCLK_edge(void) {
    hdcmi.Instance->CR ^=  DCMI_CR_PCKPOL;
    print_pixclk();
}

void print_all_param(void) {
    ptint_vs_pol();
    print_hs_pol();
    print_pixclk();
}

void case_help(void) {
    printf("\r\n\
        h - help message\r\n\
        z - start DCMI\r\n\
        x - stop DCMI\r\n\
        c - suspend DCMI\r\n\
        v - resume DCMI\r\n\
        b - toggle horizontal sync polaryty\r\n\
        n - toggle vertical sync polaryty\r\n\
        m - toggle pixel clock edge\r\n\
        p - print all parameters\r\n\
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
    case 'p': print_all_param(); break;
    
    default: break;
  }
}
