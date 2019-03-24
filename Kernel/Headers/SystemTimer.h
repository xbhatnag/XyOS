#ifndef _SYSTEM_TIMER_H_
#define _SYSTEM_TIMER_H_

#include "Peripherals.h"
#include <stdint.h>

#define SYSTEM_TIMER_BASE               (PERIPHERALS_BASE + 0x3000)
#define SYSTEM_TIMER_CONTROL_STATUS     ((volatile uint32_t *)(SYSTEM_TIMER_BASE))
#define SYSTEM_TIMER_COUNTER            ((volatile uint64_t *)(SYSTEM_TIMER_BASE + 0x4))
#define SYSTEM_TIMER_COUNTER_L32        ((volatile uint32_t *)(SYSTEM_TIMER_BASE + 0x4))
#define SYSTEM_TIMER_COUNTER_U32        ((volatile uint32_t *)(SYSTEM_TIMER_BASE + 0x8))
#define SYSTEM_TIMER_COMPARE_0          ((volatile uint32_t *)(SYSTEM_TIMER_BASE + 0xC))
#define SYSTEM_TIMER_COMPARE_1          ((volatile uint32_t *)(SYSTEM_TIMER_BASE + 0x10))
#define SYSTEM_TIMER_COMPARE_2          ((volatile uint32_t *)(SYSTEM_TIMER_BASE + 0x14))
#define SYSTEM_TIMER_COMPARE_3          ((volatile uint32_t *)(SYSTEM_TIMER_BASE + 0x18))

void system_timer_chan_1_countdown(uint32_t miliseconds);
void system_timer_chan_1_ack_interrupt();
uint32_t system_timer_chan_1_was_interrupted();

#endif
