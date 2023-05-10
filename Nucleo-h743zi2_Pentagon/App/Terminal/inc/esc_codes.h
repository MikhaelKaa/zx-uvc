// Header:
// File Name: esq_seq.c
// Author: Kargapolcev M. E.
// Date: 28.06.2019

#ifndef ESCCDS
#define ESCCDS
//#include "board.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define home()            (printf(ESC "[H")) //Move cursor to the indicated row, column (origin at 1,1)
#define clear()           (printf(ESC "[2J")) //lear the screen, move to (1,1)
#define gotoxy(x,y)       (printf(ESC "[%d;%dH", y, x))
#define visible_cursor()  (printf(ESC "[?251"))
#define resetcolor()      (printf(ESC "[0m"))
#define set_atrib(color)  (printf(ESC "[%dm", color))
//#define up()              (printf(ESC "[1A"))
//#define down()            (printf(ESC "[1B"))
#define Query_Cur_Pos()   (printf(ESC "[6n"))
#define Query_Dev_Stat()  (printf(ESC "[5n"))
#define Title()           (printf(ESC "[2<STM32 Terminal>\x07"))
#define Bell();           (printf("\x07"))

extern const uint8_t up[4];
extern const uint8_t down[4] ;
extern const uint8_t right[4];
extern const uint8_t left[4] ;
extern const uint8_t f1[6]     ;
extern const uint8_t f2[6]     ;
extern const uint8_t f3[6]     ;
extern const uint8_t f4[6]     ;
extern const uint8_t f5[6]     ;
extern const uint8_t f6[6]     ;
extern const uint8_t f7[6]     ;
extern const uint8_t f8[6]     ;
extern const uint8_t dev_stat[5];

#endif /* ESCCDS */

