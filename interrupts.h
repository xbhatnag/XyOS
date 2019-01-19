#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <stdint.h>

void ic_enable_timer_irq();
uint32_t ic_basic_irq_pending();

#endif
