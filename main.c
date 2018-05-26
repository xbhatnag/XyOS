#include <stdint.h>

enum {
  GPFSEL0 = 0x3F200000,
  GPFSET0 = 0x3F20001C,
  GPFLEV0 = 0x3F200034,
  PIN9_OUTPUT = 0x8000000,
  PIN9_SET = 0x00000200,
  PIN9_HIGH = 0x00000200
};

int main() {
  *((volatile uint32_t*)GPFSEL0) = PIN9_OUTPUT;
  *((volatile uint32_t*)GPFSET0) = PIN9_SET;
  *((volatile uint32_t*)GPFLEV0) = PIN9_HIGH;
  while(1) {}
  return 0;
}
