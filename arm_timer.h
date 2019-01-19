#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

void arm_timer_enable();
uint32_t arm_timer_is_enabled();
uint32_t arm_timer_was_interrupted();
void arm_timer_ack_interrupt();
void arm_timer_disable();
void arm_timer_countdown(uint32_t seconds);
#endif
