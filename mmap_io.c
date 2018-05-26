#include <stdint.h>
#include "mmap_io.h"

void set(int addr,int value) {
  asm("DMB");
  *(volatile uint32_t*)addr = value;
}

int get(int addr) {
  asm("DMB");
  return *(volatile uint32_t*)addr;
}
