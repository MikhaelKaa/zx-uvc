// Header:
// File Name: cursor.c
// Author: Kargapolcev M. E.
// Date: 28.06.2019

#include "cursor.h"
#include "esc_codes.h"
#include "term.h"


// Cursor
cursor_t cursor;
void cursor_update(cursor_t *curs)
{
  cursor_set(&curs->pstn);
  cursor_save(curs);
}

/*static inline*/ void cursor_save(cursor_t *curs)
{
  if(curs->pstn.x == curs->last.x) {
    curs->last.x = curs->pstn.x;
  }
  if(curs->pstn.y == curs->last.y) {
    curs->last.y = curs->pstn.y;
  }
}

void cursor_set(point_t *point)
{
  gotoxy(point->x , point->y);
}

