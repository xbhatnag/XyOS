#ifndef _GPIO_H_
#define _GPIO_H_

#include "mmap_io.h"

enum {
  GPIO_BASE = MMAP_IO_BASE + 0x200000,
  GPIO_SEL0 = GPIO_BASE,
  GPIO_SEL1 = GPIO_BASE + 0x4,
  GPIO_SEL2 = GPIO_BASE + 0x8,
  GPIO_SEL3 = GPIO_BASE + 0xC,
  GPIO_SEL4 = GPIO_BASE + 0x10,
  GPIO_SEL5 = GPIO_BASE + 0x14,
  GPIO_SET0 = GPIO_BASE + 0x1C,
  GPIO_SET1 = GPIO_BASE + 0x20,
  GPIO_CLR0 = GPIO_BASE + 0x28,
  GPIO_CLR1 = GPIO_BASE + 0x2C,
  GPIO_LEV0 = GPIO_BASE + 0x34,
  GPIO_LEV1 = GPIO_BASE + 0x38
};

enum {
  GPIO_MODE_INPUT = 0x0,
  GPIO_MODE_OUTPUT = 0x1,
  GPIO_MODE_ALT5 = 0x2,
  GPIO_SET = 0x1,
  GPIO_CLR = 0x1,
  GPIO_LEV_HIGH = 0x1,
  GPIO_LEV_LOW = 0x0,
};

void GPIO_mode(int pin, int mode);
void GPIO_clear(int pin);
void GPIO_set(int pin, int level);
#endif
