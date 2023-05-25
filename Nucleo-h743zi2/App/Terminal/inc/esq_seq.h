// Header:
// File Name: esq_seq.c
// Author: Kargapolcev M. E.
// Date: 28.06.2019

#ifndef ESCSEQ
#define ESCSEQ
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern bool esc_seq;
extern uint8_t esc_seq_buff[6];
extern uint8_t esc_seq_iter;

typedef enum {
  None,
  Up,
  Down,
  Left,
  Right,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
}esc_code_t;

void esc_seq_clear(void);
esc_code_t esc_seq_search(void);

#endif /* ESCSEQ */

