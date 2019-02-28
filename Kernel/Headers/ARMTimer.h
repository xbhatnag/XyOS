#ifndef _TIMER_H_
#define _TIMER_H_

#include "Peripherals.h"
#include <stdint.h>

#define ARM_TIMER_BASE		 (PERIPHERALS_BASE + 0xB400)
#define ARM_TIMER_LOAD		 ((volatile uint32_t*)(ARM_TIMER_BASE))
#define ARM_TIMER_VALUE		 ((volatile uint32_t*)(ARM_TIMER_BASE + 0x4))
#define ARM_TIMER_CONTROL	 ((volatile uint32_t*)(ARM_TIMER_BASE + 0x8))
#define ARM_TIMER_IRQ_ACK	 ((volatile uint32_t*)(ARM_TIMER_BASE + 0xC))
#define ARM_TIMER_RAW_IRQ	 ((volatile uint32_t*)(ARM_TIMER_BASE + 0x10))
#define ARM_TIMER_MASK_IRQ ((volatile uint32_t*)(ARM_TIMER_BASE + 0x14))

void arm_timer_enable();
uint32_t arm_timer_is_enabled();
uint32_t arm_timer_was_interrupted();
void arm_timer_ack_interrupt();
void arm_timer_disable();
void arm_timer_countdown(uint32_t seconds);
#endif
