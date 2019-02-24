#include "ARMTimer.h"

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
