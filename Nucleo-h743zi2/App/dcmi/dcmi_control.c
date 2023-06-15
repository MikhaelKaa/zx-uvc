
#include "main.h"
#include <stdio.h>
#include "dcmi.h"

extern DCMI_HandleTypeDef hdcmi;
uint8_t DCMI_flag;

void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi) {
}

void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi) {
 
}

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi) {
  DCMI_flag = 1;
  //HAL_GPIO_TogglePin(test_pin0_GPIO_Port, test_pin0_Pin);
  UVC_flag = 0;
  
}
void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi) {
  //HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

uint16_t test_offset = 0;
uint32_t last_start_len = 0;
int offset_x = 0;
int  offset_y = 0;

void dcmi_start(uint32_t len) { 
    DBG("HAL_DCMI_Start_DMA with len=%ld\r\n", len);
    int dcmi_ret = HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)dcmi_buf, (len/4));
    DBG("HAL_DCMI_Start_DMA return %d\r\n", dcmi_ret);
}

void dcmi_stop(void) {  
    int dcmi_ret = HAL_DCMI_Stop(&hdcmi);
    DBG("HAL_DCMI_Stop return %d\r\n", dcmi_ret);
}

void dcmi_suspend(void) {  
    int dcmi_ret = HAL_DCMI_Suspend(&hdcmi);
    DBG("HAL_DCMI_Suspend return %d\r\n", dcmi_ret);
}

void dcmi_resume(void) {  
    int dcmi_ret = HAL_DCMI_Resume(&hdcmi);
    DBG("HAL_DCMI_Resume return %d\r\n", dcmi_ret);
}

void print_vs_pol(void){
    if((hdcmi.Instance->CR>>DCMI_CR_VSPOL_Pos) & 1U){
        //1: DCMI_VSYNC active high
        DBG("DCMI_VSYNC active high\r\n");
    } else {
        //0: DCMI_VSYNC active low
        DBG("DCMI_VSYNC active low\r\n");
    }
}

void print_hs_pol(void) {
    if((hdcmi.Instance->CR>>DCMI_CR_HSPOL_Pos) & 1U){
        //1: DCMI_HSYNC active high
        DBG("DCMI_HSYNC active high\r\n");
    } else {
        //0: DCMI_HSYNC active low
        DBG("DCMI_HSYNC active low\r\n");
    }
}

void print_pixclk(void) {
    if((hdcmi.Instance->CR>>DCMI_CR_PCKPOL_Pos) & 1U)
        //1: Rising edge active
        DBG("DCMI pixel clock Rising edge active\r\n");
    else
        //0: Falling edge active
        DBG("DCMI pixel clock Falling edge active\r\n");
}

void dcmi_toogle_VS_polarity(void) {
    hdcmi.Instance->CR ^=  DCMI_CR_VSPOL;
    print_vs_pol();
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
    DBG("\r\n***************\r\n");
    print_vs_pol();
    print_hs_pol();
    print_pixclk();
    DBG("offset x = %d\r\n", offset_x);
    DBG("offset y = %d\r\n", offset_y);
}

void inc_test_offset(void) {
    test_offset++;
    DBG("inc test offset. TO = %d\r\n", test_offset);
}

void dec_test_offset(void) {
    test_offset--;
    DBG("dec test offset. TO = %d\r\n", test_offset);
}

void load_gmx_scorpion_set(void) {
    dcmi_stop();
    //copy_pixels = zx_copy_pix_gmx_sc;
    pix_per_line = 432;
    hdcmi.Instance->CR &=  ~DCMI_CR_VSPOL_Msk;
    hdcmi.Instance->CR |=  DCMI_CR_HSPOL_Msk;
    hdcmi.Instance->CR &=  ~DCMI_CR_PCKPOL_Msk;
    offset_x = 42;
    offset_y = 88;
    DBG("Load GMX Scorpion settings\r\n");
    //dcmi_start_gmx_sc();
    last_start_len = ZX_GMX_SCORPION_LEN;
    dcmi_start(ZX_GMX_SCORPION_LEN);
}

void load_gmx_pentagon_set(void) {
    dcmi_stop();
    //copy_pixels = zx_copy_pix_pent;
    pix_per_line = 432;
    hdcmi.Instance->CR &=  ~DCMI_CR_VSPOL_Msk;
    hdcmi.Instance->CR |=  DCMI_CR_HSPOL_Msk;
    hdcmi.Instance->CR &=  ~DCMI_CR_PCKPOL_Msk;
    offset_x = 40;
    offset_y = 88;
    DBG("Load GMX Pentagon settings\r\n");
    //dcmi_start_gmx_pent();
    last_start_len = ZX_GMX_PENTAGON_LEN;
    dcmi_start(ZX_GMX_PENTAGON_LEN);
}

void case_help(void) {
    printf("\r\n\
        h - help message\r\n\
        x - stop DCMI\r\n\
        c - suspend DCMI\r\n\
        v - resume DCMI\r\n\
        b - toggle horizontal sync polaryty\r\n\
        n - toggle vertical sync polaryty\r\n\
        m - toggle pixel clock edge\r\n\
        p - print all parameters\r\n\
        q - inc test offset\r\n\
        a - dec test offset\r\n\
        1 - load GMX Scorpion settings\r\n\
        2 - load GMX Pentagon settings\r\n\
        ");
}

void dcmi_control(uint8_t cmd) {
    switch (cmd) {
    case 'h': case_help(); break;
    case 'z': dcmi_start(last_start_len); break;
    case 'x': dcmi_stop(); break;
    case 'c': dcmi_suspend(); break;
    case 'v': dcmi_resume(); break;
    case 'b': dcmi_toogle_HS_polarity(); break;
    case 'n': dcmi_toogle_VS_polarity(); break;
    case 'm': dcmi_toogle_PIXCLK_edge(); break;
    case 'p': print_all_param(); break;
    case 'q': inc_test_offset(); break;
    case 'a': dec_test_offset(); break;
    case '1': load_gmx_scorpion_set(); break;
    case '2': load_gmx_pentagon_set(); break;
    case 'w': offset_x++; break;
    case 's': offset_x--; break;
    case 'e': offset_y++; break;
    case 'd': offset_y--; break;
    
    default: break;
  }
}
