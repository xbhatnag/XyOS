#ifndef _UART_H_
#define _UART_H_

#include "Peripherals.h"
#include <stdint.h>

#define AUX_BASE            (PERIPHERALS_BASE + 0x215000)
#define AUX_STATE           ((volatile uint32_t*)(AUX_BASE + 0x04))
#define AUX_MU_IO_REG       ((volatile uint32_t*)(AUX_BASE + 0x40))
#define AUX_MU_IER_REG      ((volatile uint32_t*)(AUX_BASE + 0x44))
#define AUX_MU_IIR_REG      ((volatile uint32_t*)(AUX_BASE + 0x48))
#define AUX_MU_LCR_REG      ((volatile uint32_t*)(AUX_BASE + 0x4C))
#define AUX_MU_MCR_REG      ((volatile uint32_t*)(AUX_BASE + 0x50))
#define AUX_MU_LSR_REG      ((volatile uint32_t*)(AUX_BASE + 0x54))
#define AUX_MU_MSR_REG      ((volatile uint32_t*)(AUX_BASE + 0x58))
#define AUX_MU_CNTL_REG     ((volatile uint32_t*)(AUX_BASE + 0x60))
#define AUX_MU_STAT_REG     ((volatile uint32_t*)(AUX_BASE + 0x64))
#define AUX_MU_BAUD         ((volatile uint32_t*)(AUX_BASE + 0x68))

void uart_setup();
void uart_flush();
void uart_output(uint32_t letter);
uint32_t uart_input();

#endif
