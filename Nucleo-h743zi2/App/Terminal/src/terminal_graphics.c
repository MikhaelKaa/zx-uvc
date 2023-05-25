// Header: Terminal grafics
// File Name: terminal_grafics.c
// Author: Kargapolcev M. E.
// Date: 26.06.2019
// Based on https://habr.com/ru/post/325082/ (dlinyj 28.03.2017-18:56)
// https://github.com/dlinyj/terminal_controller
// http://microsin.net/adminstuff/xnix/ansivt100-terminal-control-escape-sequences.html
// https://misc.flogisoft.com/bash/tip_colors_and_formatting

#include "terminal_graphics.h"
#include "term.h"
#include "esq_seq.h"
#include "esc_codes.h"
#include "cursor.h"
#include "radio_button.h"
//#include "dbg.h"
//#include "encoder.h"
//#include "multimeter.h"

void terminal_cursor(void);
void terminal_frame(void);
void terminal_header(void);
void terminal_body(void);
void terminal_basement(void);

#define BACK_GROUND       Dark_gray
#define INK               Light_green



uint8_t buff;
bool terminal_status = false;
bool terminal_redraw = false;
extern UART_HandleTypeDef huart2;

void terminal_init(void)
{
  
  home();
  clear();
  //Title(); Bell();
  terminal_frame();
  // Radio buttons
//  radbtn_led.frame = false;
  radbtn_led.state = rb_off,
  radbtn_led.point.x = 24;
  radbtn_led.point.y = 2;
  //memcpy(radbtn_led.label.text, "led ctrl", 8);
  //radbtn_led.label.point.x = 24 + 2;
  //radbtn_led.label.point.y = 2;
  //radbtn_draw(&radbtn_led);
  
  //bluetooth.frame = false;
  bluetooth.state = rb_off,
  bluetooth.point.x =  50;
  bluetooth.point.y = 2;
  //memcpy(radbtn_0.label.text, "bluetooth", strlen("bluetooth"));
  //radbtn_0.label.point.x = radbtn_0.point.x + 2;
  //radbtn_0.label.point.y = radbtn_0.point.y;
  //radbtn_draw(&radbtn_0);
  
  //radbtn_1.frame = false;
  next.state    = rb_off,
  next.point.x  = 3;
  next.point.y  = 4;
  //radbtn_1.label.point.x = radbtn_1.point.x + 2;
  //radbtn_1.label.point.y = radbtn_1.point.y;
  //memcpy(radbtn_1.label.text, "radbtn#1", 8);
  //radbtn_draw(&radbtn_1);
  
  prev.state    = rb_off,
  prev.point.x  = 2;
  prev.point.y  = 4;
  
  reboot.state = rb_off,
  reboot.point.x = 61;
  reboot.point.y = 2;
  
  // Labels
  label_debug.point.x = 2;
  label_debug.point.y = 9;
  memcpy(label_debug.text, "--------", 8);
  label_draw(&label_debug);
  
  // Cursor start position
  cursor.pstn.x = 2;
  cursor.pstn.y = 4;
  //HAL_UART_Receive_DMA(&huart2, RX, 2);
  terminal_update();
}

void terminal_update(void)
{
  /*Query_Dev_Stat();
  if(terminal_status == false) {
    terminal_redraw = true;
  }*/
  /*if(terminal_redraw) {
      home();
      clear();
      terminal_frame();
  }*/
  
  terminal_cursor();
  buff = esc_seq_search();
  terminal_header();
  terminal_body();
  terminal_basement();
  
  // Labels
  label_draw(&label_debug);
  
  // Radio buttons
  radbtn_update(&radbtn_led);
  
  if(radbtn_led.state) {
    //LD4(LED_ON);
  }else {
    //LD4(LED_OFF);
  }
  //bluetooth.state = bt_status;

  radbtn_update(&bluetooth);
  radbtn_update(&next);
  radbtn_update(&prev);
  radbtn_update(&reboot);
  if(reboot.state) NVIC_SystemReset();
  
  if(next.state) {
    //encoder.handler_next(); 
    next.state = rb_off;
  }
  
  if(prev.state) {
    //encoder.handler_prev();
    prev.state = rb_off;
  }
  cursor_update(&cursor);
}



// Callback for new byte
void terminal_callback(uint8_t *p_s)
{
  // Escape symbol find
  if(*p_s == 0x1b) {
    //new escape sequence
    esc_seq = true;
    esc_seq_buff[esc_seq_iter++] = *p_s;
  }else {
    if(esc_seq) {
      esc_seq_buff[esc_seq_iter++] = *p_s;
      if(esc_seq_iter > 5) {
        esc_seq_clear();
      }
      //buff = esc_seq_search();
      return;
    }
    // data symbols
    buff = *p_s;
  }
}

void terminal_cursor(void)
{
  switch(buff)
  {
    // Nothing to do
    case 0x00:
      break;
    // Cursor keys
    case Up:
      cursor.pstn.y--;
      break;
    case Down:
      cursor.pstn.y++;
      break;
    case Left:
      cursor.pstn.x--;
      break;
    case Right:
      cursor.pstn.x++;
      break;
    // Func keys
    case F1:
      memcpy(label_debug.text, "F1 key  ", 8);
      break;
    case F2:
      memcpy(label_debug.text, "F2 key  ", 8);
      break;
    case F3:
      memcpy(label_debug.text, "F3 key  ", 8);
      break;
    case F4:
      memcpy(label_debug.text, "F4 key  ", 8);
      break;
    case F5:
      memcpy(label_debug.text, "F5 key  ", 8);
      break;
    case F6:
      memcpy(label_debug.text, "F6 key  ", 8);
      break;
    case F7:
      memcpy(label_debug.text, "F7 key  ", 8);
      break;
    case F8:
      memcpy(label_debug.text, "F8 key  ", 8);
      Bell();
      break;

    // Space
    case ' ':
      //printf("█");
    //dbg("space");
      radbtn_check(&radbtn_led);
      radbtn_check(&bluetooth);
      radbtn_check(&next);
      radbtn_check(&prev);
      radbtn_check(&reboot);
      break;
    default:
      break;
  }
  buff = 0;
}

// Frame for GUI
void terminal_frame(void)
{
  set_atrib(BACK_GROUND);
  set_atrib(INK);
  clear();
  /* Symbol libriary */
  /* └ ┘ ┌ ┐ ─ │ ├ ┤ */
  /*
2500  ─ ━ 	│ 	┃ 	┄ 	┅ 	┆ 	┇ 	┈ 	┉ 	┊ 	┋ 	┌ 	┍ 	┎ 	┏
2510   ┐ 	┑ 	┒ 	┓ 	└ 	┕ 	┖ 	┗ 	┘ 	┙ 	┚ 	┛ 	├ 	┝ 	┞ 	┟
2520   ┠ 	┡ 	┢ 	┣ 	┤ 	┥ 	┦ 	┧ 	┨ 	┩ 	┪ 	┫ 	┬ 	┭ 	┮ 	┯
2530   ┰ 	┱ 	┲ 	┳ 	┴ 	┵ 	┶ 	┷ 	┸ 	┹ 	┺ 	┻ 	┼ 	┽ 	┾ 	┿
2540   ╀ 	╁ 	╂ 	╃ 	╄ 	╅ 	╆ 	╇ 	╈ 	╉ 	╊ 	╋ 	╌ 	╍ 	╎ 	╏
2550   ═ 	║ 	╒ 	╓ 	╔ 	╕ 	╖ 	╗ 	╘ 	╙ 	╚ 	╛ 	╜ 	╝ 	╞ 	╟
2560   ╠ 	╡ 	╢ 	╣ 	╤ 	╥ 	╦ 	╧ 	╨ 	╩ 	╪ 	╫ 	╬ 	╭ 	╮ 	╯
2570   ╰ 	╱ 	╲ 	╳ 	╴ 	╵ 	╶ 	╷ 	╸ 	╹ 	╺ 	╻ 	╼ 	╽ 	╾ 	╿
2580   ▀ 	▁ 	▂ 	▃ 	▄ 	▅ 	▆ 	▇ 	█ 	▉ 	▊ 	▋ 	▌ 	▍ 	▎ 	▏
2590   ▐ 	░ 	▒ 	▓ 	▔ 	▕ 	▖ 	▗ 	▘ 	▙ 	▚ 	▛ 	▜ 	▝ 	▞ 	▟*/
  //              1         2         3         4         5         6
  //     1234567890123456789012345678901234567890123456789012345678901
  puts( "┌────────────────────────────────────────────┬────┬────────┬─┐\n\r" //0
        "│                                            │BT: │00:00:00│ │\n\r" //1 Header
        "├────────────────────────────────────────────┴────┴────────┴─┤\n\r" //2
        "│                                                            │\n\r" //3
        "│                                                            │\n\r" //4
        "│                                                            │\n\r" //5
        "│                                                            │\n\r" //6
        "│                                                            │\n\r" //7
        "│                                                            │\n\r" //8
        "└────────────────────────────────────────────────────────────┘\n\r" //9
        "┌────────────────────────────────────────────────────────────┐\n\r" //10
        "│                                                            │\n\r" //11
        "│                                                            │\n\r" //12
        "└────────────────────────────────────────────────────────────┘");   //13
  //resetcolor();
}

void terminal_header(void)
{
  gotoxy(2,2);
  //set_atrib(BACK_GROUND);
  //set_atrib(INK);
  printf("Header");
  //resetcolor();
}

void terminal_body(void)
{
  //set_atrib(BACK_GROUND);
  //set_atrib(INK);
  gotoxy(2,5);
  //printf("%-17s", &multimeter.header[multimeter.mode][0]);
  //puts(&multimeter.header[multimeter.mode][0]);
  //puts("            ");
  gotoxy(2,6);
  //printf("%-18s", multimeter.value);
  //puts(multimeter.value);
  //puts("            ");
  gotoxy(2,7);
  //printf("%-18s", multimeter.debug);
  //puts(multimeter.debug);
  //puts("            ");
  gotoxy(2,8);
  //printf("%-18s", multimeter.base);
  //puts(multimeter.base);
  //puts("            ");
}

void terminal_basement(void)
{
  //set_atrib(BACK_GROUND);
  //set_atrib(INK);
  gotoxy(2, 12);
  printf("cursor.pstn.x: %d  ", cursor.pstn.x);
  gotoxy(2, 13);
  printf("cursor.pstn.y: %d  ", cursor.pstn.y);
}
