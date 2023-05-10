// Header:
// File Name: cursor.c
// Author: Kargapolcev M. E.
// Date: 28.06.2019

#ifndef CURSOR
#define CURSOR
//#include "board.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Point on screen
typedef struct {
  uint8_t x;
  uint8_t y;
} point_t;

// Label
typedef struct {
  point_t point;
  uint8_t text[12];
} label_t;

// Cursor
typedef struct {
  // Position
  point_t pstn;
  // Last position
  point_t last;
}cursor_t;
extern cursor_t cursor;

/*static inline*/ void cursor_save(cursor_t *curs);
void cursor_update(cursor_t *curs);
void cursor_set(point_t *point);


#endif /* CURSOR */

