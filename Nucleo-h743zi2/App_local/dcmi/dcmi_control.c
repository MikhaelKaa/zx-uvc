/**
 * @file dcmi_control.c
 * @brief DCMI STM32 Capture dev
 * @author Mikhael Kaa (Михаил Каа)
 * @date 20.07.2025
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "main.h"
#include "dcmi.h"
#include "micros.h"
#include "zx_capture.h"

#include "event_meter.h"

#ifdef BAREMETAL
#define ENDL "\r\n"
#else
#define ENDL "\n"
#endif // BAREMETAL

extern DCMI_HandleTypeDef hdcmi;
uint8_t DCMI_flag;
uint8_t uvc_cnt = 0;

event_meter_t vsync = EVENT_METER_INIT("vsync");
event_meter_t hsync = EVENT_METER_INIT("hsync");
event_meter_t frame = EVENT_METER_INIT("frame");

static volatile uint32_t dcmi_errors_event_cnt = 0;
static volatile uint32_t dcmi_line_cnt = 0;
static volatile uint32_t dcmi_line = 0;

// uint16_t zx_buf_len = 1 * ((((sizeof(zx_buf_pent)+0) / 2) / 4) - 3198);  // = 28418
uint16_t zx_h_len = 384; // "константа" строки для желтого скорпа.
uint32_t zx_buf_len = (384 * 296) * 2 + 12; // 227340

volatile int offset_x = 0;
volatile int offset_y = 0;

void print_usage(void);
void dcmi_show_settings(void);
void dcmi_control(uint8_t cmd);

int ucmd_dcmi(int argc, char ** argv) {

    uint32_t offset = 0;
    int temp = 0;

    switch (argc) {
    case 1:
        print_usage();
        return -EINVAL;

    case 2:
        if (strcmp(argv[1], "stat") == 0) {
            dcmi_show_settings();
            event_meter_show(&hsync);
            event_meter_show(&vsync);
            event_meter_show(&frame);
            
            printf("dcmi_errors_event_cnt = %lu" ENDL, dcmi_errors_event_cnt);
            printf("dcmi_line = %lu" ENDL, dcmi_line);
            printf("zx_h_len = %u" ENDL, zx_h_len);
            
            return 0;
        }
    
    case 3:
        if (strcmp(argv[1], "start") == 0) {
            if (sscanf(argv[2], "%lu", &offset) != 1) {
                printf("Invalid offset format" ENDL);
                return -EINVAL;
            }
            zx_buf_len = offset;
            dcmi_control('3');
            return 0;
        }
        
        if (strcmp(argv[1], "offx") == 0) {
            if (sscanf(argv[2], "%d", &temp) != 1) {
                printf("Invalid offset_x format" ENDL);
                return -EINVAL;
            }
            offset_x = temp;
            printf("offset_x = %d" ENDL, offset_x);
            dcmi_control('3');
            return 0;
        }
        
        if (strcmp(argv[1], "offy") == 0) {
            if (sscanf(argv[2], "%d", &temp) != 1) {
                printf("Invalid offset_y format" ENDL);
                return -EINVAL;
            }
            offset_y = temp;
            printf("offset_y = %d" ENDL, offset_y);
            dcmi_control('3');
            return 0;
        }

        break;

    case 4:
        if (strcmp(argv[1], "start") == 0) {
            if (sscanf(argv[2], "%lu", &offset) != 1) {
                printf("Invalid offset format" ENDL);
                return -EINVAL;
            }
            if (sscanf(argv[3], "%hu", &zx_h_len) != 1) {
                printf("Invalid zx_h_len format" ENDL);
                return -EINVAL;
            }
            zx_buf_len = offset;
            dcmi_control('3');
            return 0;
        }
        break;
        
    default:
        print_usage();
        return -EINVAL;
    }
    
    print_usage();
    return -EINVAL;
}


// HSync
void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi) {
    event_meter_record(&hsync, micros());
    dcmi_line_cnt++;
}

extern uint8_t* uvc_frame;
// HSync
void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi) {
    event_meter_record(&vsync, micros());
    dcmi_line = dcmi_line_cnt;
    dcmi_line_cnt = 0;
    // 
    DCMI_flag = 1;
    uvc_frame = 0;
}

// 
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi) {
    event_meter_record(&frame, micros());
}

// 
void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi) {
  dcmi_errors_event_cnt++;
}


// TODO:
// ((sizeof(zx_buf_pent)/4) - 3198) = 28418
// (sizeof(zx_buf_pent)/4) = 31616



void dcmi_start(void) {  
    int dcmi_ret = HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)zx_buf_pent, zx_buf_len / 4);
    printf("HAL_DCMI_Start return %d" ENDL, dcmi_ret);
}

void dcmi_stop(void) {  
    int dcmi_ret = HAL_DCMI_Stop(&hdcmi);
    printf("HAL_DCMI_Stop return %d" ENDL, dcmi_ret);
}

void dcmi_suspend(void) {  
    int dcmi_ret = HAL_DCMI_Suspend(&hdcmi);
    printf("HAL_DCMI_Suspend return %d" ENDL, dcmi_ret);
}

void dcmi_resume(void) {  
    int dcmi_ret = HAL_DCMI_Resume(&hdcmi);
    printf("HAL_DCMI_Resume return %d" ENDL, dcmi_ret);
}

void print_vs_pol(void){
    if((hdcmi.Instance->CR>>DCMI_CR_VSPOL_Pos) & 1U){
        //1: DCMI_VSYNC active high
        printf("DCMI_VSYNC active high" ENDL);
    } else {
        //0: DCMI_VSYNC active low
        printf("DCMI_VSYNC active low" ENDL);
    }
}

void print_hs_pol(void) {
    if((hdcmi.Instance->CR>>DCMI_CR_HSPOL_Pos) & 1U){
        //1: DCMI_HSYNC active high
        printf("DCMI_HSYNC active high" ENDL);
    } else {
        //0: DCMI_HSYNC active low
        printf("DCMI_HSYNC active low" ENDL);
    }
}

void print_pixclk(void) {
    if((hdcmi.Instance->CR>>DCMI_CR_PCKPOL_Pos) & 1U)
        //1: Rising edge active
        printf("DCMI pixel clock Rising edge active" ENDL);
    else
        //0: Falling edge active
        printf("DCMI pixel clock Falling edge active" ENDL);
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

void dcmi_show_settings(void) {
    print_vs_pol();
    print_hs_pol();
    print_pixclk();
    printf("offset x = %d" ENDL, offset_x);
    printf("offset y = %d" ENDL, offset_y);
}

void load_gmx_scorpion_set(void) {
    dcmi_stop();
    copy_pixels = zx_copy_pix_gmx_sc;
    hdcmi.Instance->CR &=  ~DCMI_CR_VSPOL_Msk;
    hdcmi.Instance->CR |=  DCMI_CR_HSPOL_Msk;
    hdcmi.Instance->CR &=  ~DCMI_CR_PCKPOL_Msk;
    offset_x = 42;
    offset_y = 88;
    printf("Load GMX Scorpion settings" ENDL);
    dcmi_start();
}

void load_gmx_pentagon_set(void) {
    dcmi_stop();
    copy_pixels = zx_copy_pix_gmx_pent;
    hdcmi.Instance->CR |=  DCMI_CR_VSPOL_Msk;
    hdcmi.Instance->CR &=  ~DCMI_CR_HSPOL_Msk;
    hdcmi.Instance->CR &=  ~DCMI_CR_PCKPOL_Msk;
    offset_x = 40;
    offset_y = 88;
    printf("Load GMX Pentagon settings" ENDL);
    dcmi_start();
}

void load_scorp_y_set(void) {
    dcmi_stop();
    copy_pixels = zx_copy_pix_uni;
    hdcmi.Instance->CR &=  ~DCMI_CR_VSPOL_Msk;
    hdcmi.Instance->CR &=  ~DCMI_CR_HSPOL_Msk;
    hdcmi.Instance->CR &=  ~DCMI_CR_PCKPOL_Msk;
    offset_x = 42;
    offset_y = -32;
    printf("Load Scorpion settings" ENDL);
    printf("zx_buf_len = %lu" ENDL, zx_buf_len);
    printf("zx_h_len = %u" ENDL, zx_h_len);
    dcmi_start();
}

void print_usage(void) {
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
    case 'h': print_usage(); break;
    case 'z': dcmi_start(); break;
    case 'x': dcmi_stop(); break;
    case 'c': dcmi_suspend(); break;
    case 'v': dcmi_resume(); break;
    case 'b': dcmi_toogle_HS_polarity(); break;
    case 'n': dcmi_toogle_VS_polarity(); break;
    case 'm': dcmi_toogle_PIXCLK_edge(); break;
    case 'p': dcmi_show_settings(); break;
    case '1': load_gmx_scorpion_set(); break;
    case '2': load_gmx_pentagon_set(); break;
    case '3': load_scorp_y_set(); break;
    case 'w': offset_x++; break;
    case 's': offset_x--; break;
    case 'e': offset_y++; break;
    case 'd': offset_y--; break;
    
    default: break;
  }
}
