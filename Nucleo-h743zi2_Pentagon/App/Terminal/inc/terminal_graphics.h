// Header: Terminal grafics
// File Name: terminal_grafics.h
// Author: Kargapolcev M. E.
// Date: 26.06.2019
// Based on https://habr.com/ru/post/325082/ (dlinyj 28.03.2017-18:56)
// https://github.com/dlinyj/terminal_controller

#ifndef TG
#define TG
#include "main.h"

extern uint8_t RX[];

void terminal_init(void);
void terminal_update(void);
void terminal_callback(uint8_t *p_s);
#endif /* TG*/
