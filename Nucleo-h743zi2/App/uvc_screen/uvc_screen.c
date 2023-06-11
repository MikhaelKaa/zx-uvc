#include "uvc_screen.h"
#include "uvc_font.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "retarget.h"
#include "rgb565_color.h"

void uvc_cls(void);
void uvc_fill(uint16_t color);

void uvc_render_char(uint16_t pos_x, uint16_t pos_y, uint8_t num);
void uvc_redner_str(uint8_t x, uint8_t y, const char* buf);

const uint32_t size_y = UVC_VIDEO_HEIGHT/8;
const uint32_t size_x = UVC_VIDEO_WIDTH/8;
char text_buf[UVC_VIDEO_HEIGHT/8][UVC_VIDEO_WIDTH/8];
int text_buf_sizeof = sizeof(text_buf);
static uint16_t uvc_screen_y = 0;

void uvc_render_text_buf(void) {
    uvc_fill(UVC_PAPER);
    for(int y = 0; y < size_y; y++) {
        uvc_redner_str(0, y*8, &text_buf[((y+uvc_screen_y+1)%size_y)][0]);
    }
}

//void uvc_screen_printf(const char* format, ...) {
void uvc_screen_printf(const char* format) {
    //char temp_buf[32];
    //va_list args;
    //va_start(args, format);
    //sprintf(temp_buf, format, args);
    //va_end(args);
    strcpy(&text_buf[(uvc_screen_y--%size_y)][0], format);
}

void uvc_cls(void) {
    memset(ucv_buf, 0x00, sizeof(ucv_buf));
}

void uvc_fill(uint16_t color) {
    uint16_t *buf = &ucv_buf[0][0];
    size_t count = (sizeof(ucv_buf)/2);
    while(count--) *buf++ = color;
}

void uvc_redner_str(uint8_t x, uint8_t y, const char* buf) {
    int len = strlen(buf);
    for(int n = 0; n < len; n++) {
        if((buf[n] >= 32) && (buf[n] <= 122)) uvc_render_char(x+n*8, y, buf[n] - 32);
    }
}

void uvc_render_char(uint16_t pos_x, uint16_t pos_y, uint8_t num) {
    // Условия выхода за границы не проверяю. 
    //if((pos_x - 8) > UVC_VIDEO_WIDTH) pos_x = UVC_VIDEO_WIDTH-8;
    //if((pos_y - 8) > UVC_VIDEO_HEIGHT) pos_y = UVC_VIDEO_HEIGHT-8;
    
    for(int8_t y = 0; y < 8; y++) {
        for(uint8_t x = 0; x < 8; x++) {
            ucv_buf[pos_y + 8-1 - y][pos_x + x] = ((font[8*num + y]<<x)&0x80)?(UVC_INK):(UVC_PAPER);
        }
    }
}

void UVC_SCREEN_test(void) {
    char buf[31];

    /*uvc_cls();
    HAL_Delay(100);

    sprintf(buf, "%s", "!\"#\$\%&'()*+,-./:;<=>?@[\\]^_`");
    uvc_redner_str(0, 96, buf);
    HAL_Delay(100);

    sprintf(buf, "%s", "0 1 2 3 4 5 6 7 8 9");
    uvc_redner_str(0, 80, buf);
    HAL_Delay(100);

    sprintf(buf, "%s", "APPER CASE TEST");
    uvc_redner_str(0, 64, buf);
    HAL_Delay(100);

    sprintf(buf, "%s", "lower case test");
    uvc_redner_str(0, 48, buf);
    HAL_Delay(100);

    static int num = 0;
    sprintf(buf, "%i", num++);
    uvc_redner_str(0, 32, buf);
    HAL_Delay(100);

    sprintf(buf, "float test %f", 3.141592654);
    uvc_redner_str(0, 16, buf);
    HAL_Delay(100);*/

    HAL_Delay(2000);
    uvc_fill(UVC_PAPER);

    HAL_Delay(10);

    for(int i = 0; i <= size_y; i++) {
        sprintf(buf, "num %i", i);
        strcpy(&text_buf[i][0], buf);
    }
    uvc_render_text_buf();

}