#ifndef _UART_H_
#define _UART_H_

#include "mmap_io.h"

enum {
  AUX_BASE = MMAP_IO_BASE + 0x215000,
  AUX_STATE = AUX_BASE + 0x4,
  AUX_MU_IO_REG = AUX_BASE + 0x40,
  AUX_MU_IER_REG = AUX_BASE + 0x44,
  AUX_MU_IIR_REG = AUX_BASE + 0x48,
  AUX_MU_LCR_REG = AUX_BASE + 0x4C,
  AUX_MU_MCR_REG = AUX_BASE + 0x50,
  AUX_MU_LSR_REG = AUX_BASE + 0x54,
  AUX_MU_MSR_REG = AUX_BASE + 0x58,
  AUX_MU_CNTL_REG = AUX_BASE + 0x60,
  AUX_MU_STAT_REG = AUX_BASE + 0x64,
  AUX_MU_BAUD = AUX_BASE + 0x68
};

void UART_setup();
void UART_flush();
void UART_output(char letter);

#endif
