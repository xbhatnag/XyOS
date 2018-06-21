#include "stdio.h"
#include "mmap_io.h"

#define TIMER_BASE		 (MMAP_IO_BASE + 0xB400)
#define TIMER_LOAD		 ((volatile unsigned *)(TIMER_BASE))
#define TIMER_VALUE		 ((volatile unsigned *)(TIMER_BASE + 0x4))
#define TIMER_CONTROL		 ((volatile unsigned *)(TIMER_BASE + 0x8))
#define TIMER_IRQ_ACK		 ((volatile unsigned *)(TIMER_BASE + 0xC))
#define TIMER_RAW_IRQ		 ((volatile unsigned *)(TIMER_BASE + 0x10))
#define TIMER_MASK_IRQ		 ((volatile unsigned *)(TIMER_BASE + 0x14))

void timer_enable() {
  println("Enabling ARM Timer...");
  *TIMER_CONTROL = 0x3E00aa;
}

int timer_isEnabled() {
  int val = *TIMER_CONTROL;
  if (val & (1 << 7)) {
  	return 1;
  } else {
  	return 0;
  }
}

int timer_interrupted() {
  return *TIMER_RAW_IRQ;
}

int timer_getValue() {
  return *TIMER_VALUE;
}

void countdown(int miliseconds) {
  *TIMER_LOAD = miliseconds;
  while(timer_getValue() != 0) {} 
}
