#ifndef _SYSTEM_TIMER_H_
#define _SYSTEM_TIMER_H_

#include <stdint.h> 

void system_timer_chan_1_countdown(uint32_t miliseconds);
void system_timer_chan_1_ack_interrupt();
uint32_t system_timer_chan_1_was_interrupted();

#endif
