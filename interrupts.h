#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <stdint.h>

void ic_enable_arm_timer_irq();
void ic_enable_system_timer_chan_1_irq();
uint32_t ic_basic_irq_pending();

#endif
