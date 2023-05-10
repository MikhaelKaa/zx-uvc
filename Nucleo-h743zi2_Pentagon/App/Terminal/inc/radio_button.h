// Header:
// File Name: cursor.c
// Author: Kargapolcev M. E.
// Date: 28.06.2019

#ifndef RDBTN
#define RDBTN
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cursor.h"

// Switch
typedef enum {
  rb_off,
  rb_on,
}state_t;
typedef struct {
  //bool    frame;
  state_t state;
  point_t point;
  //label_t label;
} radbtn_t;
extern radbtn_t radbtn_led;
extern radbtn_t bluetooth;
extern radbtn_t next;
extern radbtn_t prev;
extern radbtn_t reboot;
extern label_t label_debug;

void label_draw(label_t *lb);
void radbtn_draw(radbtn_t *sw);
void radbtn_update(radbtn_t *sw);
void radbtn_check(radbtn_t *sw);
void radbtn_toggle(radbtn_t *sw);

#endif /* RDBTN */

