// Header:
// File Name: esc_codes.c
// Author: Kargapolcev M. E.
// Date: 28.06.2019

#include "esc_codes.h"
#include "main.h"

const uint8_t up[4]     = {0x1b, 0x5b, 0x41, 0x00};
const uint8_t down[4]   = {0x1b, 0x5b, 0x42, 0x00};
const uint8_t right[4]  = {0x1b, 0x5b, 0x43, 0x00};
const uint8_t left[4]   = {0x1b, 0x5b, 0x44, 0x00};
const uint8_t f1[6]     = {0x1b, 0x5b, 0x31, 0x31, 0x7e, 0x00};
const uint8_t f2[6]     = {0x1b, 0x5b, 0x31, 0x32, 0x7e, 0x00};
const uint8_t f3[6]     = {0x1b, 0x5b, 0x31, 0x33, 0x7e, 0x00};
const uint8_t f4[6]     = {0x1b, 0x5b, 0x31, 0x34, 0x7e, 0x00};
const uint8_t f5[6]     = {0x1b, 0x5b, 0x31, 0x35, 0x7e, 0x00};
const uint8_t f6[6]     = {0x1b, 0x5b, 0x31, 0x37, 0x7e, 0x00};
const uint8_t f7[6]     = {0x1b, 0x5b, 0x31, 0x38, 0x7e, 0x00};
const uint8_t f8[6]     = {0x1b, 0x5b, 0x31, 0x39, 0x7e, 0x00};
const uint8_t dev_stat[5] = {0x1b, 0x5b, 0x30, 0x6e, 0x00};