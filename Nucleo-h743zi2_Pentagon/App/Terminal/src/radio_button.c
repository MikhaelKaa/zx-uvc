// Header:
// File Name: radio_button.c
// Author: Kargapolcev M. E.
// Date: 28.06.2019

#include "radio_button.h"
#include "esc_codes.h"

radbtn_t radbtn_led;
radbtn_t next;
radbtn_t prev;
radbtn_t bluetooth;
radbtn_t reboot;

label_t label_debug;
// Labels
void label_draw(label_t *lb)
{
  //return;
  cursor_set(&lb->point);
  printf("%s", lb->text);
  //cursor_set(&cursor.pstn);
}
void radbtn_draw(radbtn_t *sw)
{
  //label_draw(&sw->label);
  /*if(!sw->frame) return;
  cursor_set(&sw->point);
  point_t temp;
  temp.x = sw->point.x - 1;
  temp.y = sw->point.y - 1;
  cursor_set(&temp);
  printf("%s", "┌─┐");
  temp.x = sw->point.x - 1;
  temp.y = sw->point.y;
  cursor_set(&temp);
  printf("%s", "│ │");
  temp.x = sw->point.x - 1;
  temp.y = sw->point.y + 1;
  cursor_set(&temp);
  printf("%s", "└─┘");
  //cursor_set(&cursor.pstn);*/
}

void radbtn_update(radbtn_t *sw)
{
  cursor_set(&sw->point);
  if(sw->state) {
    //printf("%s", "█");
    puts("█");
  }else {
    //printf("%s", "░");
    puts("▒");
  }
}

void radbtn_check(radbtn_t *sw)
{
  if(sw->point.x == cursor.pstn.x &&\
   sw->point.y == cursor.pstn.y) {
    radbtn_toggle(sw);
  }
}

void radbtn_toggle(radbtn_t *sw)
{
  sw->state = !sw->state;
}
