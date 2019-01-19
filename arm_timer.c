#include "mmap_io.h"
#include "arm_timer.h"

#define ARM_TIMER_BASE		 (MMAP_IO_BASE + 0xB400)
#define ARM_TIMER_LOAD		 ((uint32_t *)(ARM_TIMER_BASE))
#define ARM_TIMER_VALUE		 ((uint32_t *)(ARM_TIMER_BASE + 0x4))
#define ARM_TIMER_CONTROL	 ((uint32_t *)(ARM_TIMER_BASE + 0x8))
#define ARM_TIMER_IRQ_ACK	 ((uint32_t *)(ARM_TIMER_BASE + 0xC))
#define ARM_TIMER_RAW_IRQ	 ((uint32_t *)(ARM_TIMER_BASE + 0x10))
#define ARM_TIMER_MASK_IRQ	 ((uint32_t *)(ARM_TIMER_BASE + 0x14))

void arm_timer_enable() {
	*ARM_TIMER_CONTROL = 0xAA;
	*ARM_TIMER_IRQ_ACK = 0;
}

uint32_t arm_timer_is_enabled() {
	if (*ARM_TIMER_CONTROL & 0xA0) {
		return 1;
	} else {
		return 0;
	}
}

uint32_t arm_timer_was_interrupted() {
	return *ARM_TIMER_MASK_IRQ;
}

void arm_timer_ack_interrupt() {
	*ARM_TIMER_IRQ_ACK = 0;
}

void arm_timer_disable() {
	*ARM_TIMER_CONTROL = 0;
}

void arm_timer_countdown(uint32_t miliseconds) {
	// This multiplier is completely arbitrary
	// The clock speed for this timer changes dynamically
	*ARM_TIMER_LOAD = miliseconds * 4;
}
