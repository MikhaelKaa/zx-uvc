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

#ifdef BAREMETAL
#define ENDL "\r\n"
#else
#define ENDL "\n"
#endif // BAREMETAL

extern DCMI_HandleTypeDef hdcmi;
uint8_t DCMI_flag;
uint8_t uvc_cnt = 0;

#define METR_BLEN (16)
typedef struct meter {
uint32_t event_time_now;
uint32_t event_time_last;
uint32_t event_diff_cnt;
uint32_t event_time_diff[16];
char name[16];
}event_time_meter_t;

event_time_meter_t vsync = {0, 0, 0, {0}, "vsync"};
event_time_meter_t line  = {0, 0, 0, {0}, "hsync"};
event_time_meter_t frame = {0, 0, 0, {0}, "frame"};
static volatile uint32_t dcmi_errors_event_cnt = 0;
static volatile uint32_t dcmi_line_cnt = 0;
static volatile uint32_t dcmi_line = 0;

uint16_t test_offset = 3198;
volatile int offset_x = 0;
volatile int offset_y = 0;

void print_usage(void);
void dcmi_show_settings(void);
void dcmi_control(uint8_t cmd);

static inline void dcmi_meter_proc(event_time_meter_t* meter);
void dcmi_show_meters(event_time_meter_t* meter);

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
            dcmi_show_meters(&vsync);
            dcmi_show_meters(&line);
            dcmi_show_meters(&frame);
            printf("dcmi_errors_event_cnt = %lu" ENDL, dcmi_errors_event_cnt);
            printf("dcmi_line = %lu" ENDL, dcmi_line);
            printf("zx_x = %u" ENDL, zx_x);
            
            return 0;
        }
    
    case 3:
        if (strcmp(argv[1], "start") == 0) {
            if (sscanf(argv[2], "%lu", &offset) != 1) {
                printf("Invalid offset format" ENDL);
                return -EINVAL;
            }
            test_offset = offset;
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
            if (sscanf(argv[3], "%hu", &zx_x) != 1) {
                printf("Invalid zx_x format" ENDL);
                return -EINVAL;
            }
            test_offset = offset;
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



void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi) {
    dcmi_meter_proc(&vsync);
    dcmi_line = dcmi_line_cnt;
    dcmi_line_cnt = 0;
}

void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi) {
    dcmi_meter_proc(&line);
    dcmi_line_cnt++;
}

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi) {
    dcmi_meter_proc(&frame);
  DCMI_flag = 1;
    uvc_cnt++;
  //HAL_GPIO_TogglePin(test_pin0_GPIO_Port, test_pin0_Pin);
  UVC_flag = 0;
  
}
void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi) {
  dcmi_errors_event_cnt++;
}



// TODO:
void dcmi_start_gmx_sc(void) {
    int dcmi_ret = HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)zx_buf_pent, (sizeof(zx_buf_pent)/4) - test_offset);
    printf("HAL_DCMI_Start_DMA return %d" ENDL, dcmi_ret);
}

void dcmi_start_gmx_pent(void) {
    int dcmi_ret = HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)zx_buf_pent, (sizeof(zx_buf_pent)/4) - test_offset);
    printf("HAL_DCMI_Start_DMA return %d" ENDL, dcmi_ret);
}

void dcmi_start_pent(void) {
    int dcmi_ret = HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)zx_buf_pent, (sizeof(zx_buf_pent)/4) - test_offset);
    printf("HAL_DCMI_Start_DMA return %d" ENDL, dcmi_ret);
}
void dcmi_start(void) {  
    int dcmi_ret = HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)zx_buf_pent, (sizeof(zx_buf_pent)/4) - test_offset);
    printf("HAL_DCMI_Stop return %d" ENDL, dcmi_ret);
    printf("WARNING!!! start with zx_buf_gmx_sc !!! for test only" ENDL);
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

static inline void dcmi_meter_proc(event_time_meter_t* meter) {
    meter->event_time_now = micros();
    meter->event_time_diff[meter->event_diff_cnt++ %METR_BLEN] = meter->event_time_now - meter->event_time_last;
    meter->event_time_last = meter->event_time_now;
}

void dcmi_show_meters(event_time_meter_t* meter) {

    uint32_t _max = 0;
    uint32_t _min = UINT32_MAX;
    uint64_t _avg = 0;

    if(meter->event_diff_cnt < METR_BLEN) {
        printf("%s: No measurements yet\n", meter->name);
        return;
    }

    for(uint32_t i = 0; i < METR_BLEN; i++) {
        // printf("%s->event_time_diff[%lu] = %lu us" ENDL, meter->name, i, meter->event_time_diff[i]);
        if(meter->event_time_diff[i] >= _max)  _max = meter->event_time_diff[i];
        if(meter->event_time_diff[i] <= _min)  _min = meter->event_time_diff[i];
        _avg += meter->event_time_diff[i];
    }
    _avg /= METR_BLEN;
    // printf("%s statistics" ENDL, meter->name);
    printf("%s max = %lu us" ENDL,meter->name, _max);
    printf("%s min = %lu us" ENDL,meter->name, _min);
    printf("%s avg = %lu us" ENDL,meter->name, (uint32_t)_avg);
}

void inc_test_offset(void) {
    test_offset++;
    printf("inc test offset. TO = %d" ENDL, test_offset);
}

void dec_test_offset(void) {
    test_offset--;
    printf("dec test offset. TO = %d" ENDL, test_offset);
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
    dcmi_start_gmx_sc();
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
    printf("test_offset = %u" ENDL, test_offset);
    printf("zx_x = %u" ENDL, zx_x);
    
    dcmi_start_gmx_sc();
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
    dcmi_start_gmx_pent();
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
    case 'q': inc_test_offset(); break;
    case 'a': dec_test_offset(); break;
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
