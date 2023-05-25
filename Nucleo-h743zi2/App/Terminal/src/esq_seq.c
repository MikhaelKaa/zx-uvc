// Header:
// File Name: esq_seq.c
// Author: Kargapolcev M. E.
// Date: 28.06.2019

#include "esq_seq.h"
#include "esc_codes.h"

extern uint8_t buff;
extern bool terminal_status;
extern bool terminal_redraw ;

bool esc_seq = false;
uint8_t esc_seq_buff[6] = {0,0,0,0,0,0};
uint8_t esc_seq_iter = 0;

void esc_seq_clear(void)
{
  memset(esc_seq_buff, 0, 6);
  esc_seq = false;
  esc_seq_iter = 0;
}

esc_code_t esc_seq_search(void)
{
  terminal_status = false;
  if(strcmp((char*)esc_seq_buff, (char*)up) == 0) {
    esc_seq_clear();
    return Up;}
  if(strcmp((char*)esc_seq_buff,(char*) down) == 0) {
    esc_seq_clear();
    return Down;}
  if(strcmp((char*)esc_seq_buff, (char*)left) == 0) {
    esc_seq_clear();
    return Left;}
  if(strcmp((char*)esc_seq_buff, (char*)right) == 0) {
    esc_seq_clear();
    return Right;}
  if(strcmp((char*)esc_seq_buff, (char*)f1) == 0) {
    esc_seq_clear();
    return F1;}
  if(strcmp((char*)esc_seq_buff, (char*)f2) == 0) {
    esc_seq_clear();
    return F2;}
  if(strcmp((char*)esc_seq_buff, (char*)f3) == 0) {
    esc_seq_clear();
    return F3;}
  if(strcmp((char*)esc_seq_buff, (char*)f4) == 0) {
    esc_seq_clear();
    return F4;}
  if(strcmp((char*)esc_seq_buff, (char*)f5) == 0) {
    esc_seq_clear();
    return F5;}
  if(strcmp((char*)esc_seq_buff, (char*)f6) == 0) {
    esc_seq_clear();
    return F6;}
  if(strcmp((char*)esc_seq_buff, (char*)f7) == 0) {
    esc_seq_clear();
    return F7;}
  if(strcmp((char*)esc_seq_buff, (char*)f8) == 0) {
    esc_seq_clear();
    //Bell();
    return F8;}
  if(strcmp((char*)esc_seq_buff, (char*)dev_stat) == 0) {
    //terminal_status = true;
    //terminal_redraw = false;
    esc_seq_clear();
    return None;}

  return None;
}
