#ifndef UVC_SCREEN
#define UVC_SCREEN

#include "main.h"

void UVC_SCREEN_test(void);
void uvc_render_text_buf(void);

void uvc_screen_printf(const char* format);
//void uvc_screen_printf (char const *fmt, ...); 
//#define uvc_printf(...) uvc_screen_printf(__VA_ARGS__)

#endif // UVC_SCREEN 