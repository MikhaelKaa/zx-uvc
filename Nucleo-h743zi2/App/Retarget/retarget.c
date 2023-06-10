//01.11.2020
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "retarget.h"

// printf buffer
#define BUF_SZ  (4096*2)
RAM_D1 uint8_t  buf[BUF_SZ];
volatile int counter = 0U;

// uart на отладочной плате
extern UART_HandleTypeDef huart3;

// Circular update
void printf_flush(void)
{
  if(counter)
  {
    int size = counter;
    if(HAL_BUSY == HAL_UART_Transmit_DMA(&huart3, buf, size)) return;
    counter = 0;
  }
}

int _write(int fd, char* ptr, int len)
{
  (void)fd;
  int i = 0;
  while (ptr[i] && (i < len)) {
    buf[counter++%BUF_SZ] = (uint8_t)ptr[i++];
  }
  return len;
}
