#include "uvc_screen.h"
#include "uvc_font.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "retarget.h"

void uvc_cls(void);
void uvc_draw_char(uint16_t pos_x, uint16_t pos_y, uint8_t char_num);
void uvc_printf(uint8_t x, uint8_t y, const char* format, ...);

void uvc_cls(void) {
    memset(ucv_buf, 0x00, sizeof(ucv_buf));
}

void uvc_draw_char(uint16_t pos_x, uint16_t pos_y, uint8_t char_num) {
    // Условия выхода за границы не проверяю. 
    //if((pos_x - 8) > UVC_VIDEO_WIDTH) pos_x = UVC_VIDEO_WIDTH-8;
    //if((pos_y - 8) > UVC_VIDEO_HEIGHT) pos_y = UVC_VIDEO_HEIGHT-8;
    
    for(int8_t y = 0; y < 8; y++) {
        for(uint8_t x = 0; x < 8; x++) {
            ucv_buf[pos_y + 8 - y][pos_x + x] = ((font[8*char_num + y]<<x)&0x80)?(0xffff):(0x0000);
        }
    }
}

void uvc_printf(uint8_t x, uint8_t y, const char* format, ...) {
    va_list argptr;
    va_start(argptr, format);
    char buf[31];
    sprintf(buf, format, argptr);
    va_end(argptr);
    int len = strlen(buf);
    for(int n = 0; n < len; n++) {
        const uint8_t num_offset = 0;
        if((buf[n] >= 32) && (buf[n] <= 122)) uvc_draw_char(x+n*8, y, num_offset + buf[n] - 32);
    }
    
}

void UVC_SCREEN_test(void) {
    char buf[31];

    uvc_cls();
    HAL_Delay(100);

    sprintf(buf, "%s", "!\"#\$\%&'()*+,-./:;<=>?@[\\]^_`");
    uvc_printf(0, 96, buf);
    HAL_Delay(100);

    sprintf(buf, "%s", "0 1 2 3 4 5 6 7 8 9");
    uvc_printf(0, 80, buf);
    HAL_Delay(100);

    sprintf(buf, "%s", "APPER CASE TEST");
    uvc_printf(0, 64, buf);
    HAL_Delay(100);

    sprintf(buf, "%s", "lower case test");
    uvc_printf(0, 48, buf);
    HAL_Delay(100);

    static int num = 0;
    sprintf(buf, "%i", num++);
    uvc_printf(0, 32, buf);
    HAL_Delay(100);

    sprintf(buf, "float test %f", 3.141592654);
    uvc_printf(0, 16, buf);

    HAL_Delay(500);
}