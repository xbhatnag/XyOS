#include "InterruptController.h"

void ic_enable_arm_timer_irq() {
	*IC_ENABLE_BASIC_IRQ |= 1;
}

void ic_enable_system_timer_chan_1_irq() {
	*IC_ENABLE_IRQ_1 |= 2;
}

uint32_t ic_basic_irq_pending() {
	return *IC_IRQ_BASIC_PENDING;
}
