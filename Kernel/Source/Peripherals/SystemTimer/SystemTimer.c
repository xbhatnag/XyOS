#include "SystemTimer.h"

void system_timer_chan_1_countdown(uint32_t miliseconds) {
	*SYSTEM_TIMER_COMPARE_1 = ((*SYSTEM_TIMER_COUNTER_L32) + (1000 * miliseconds));
}

void system_timer_chan_1_ack_interrupt() {
	*SYSTEM_TIMER_CONTROL_STATUS = 2;
}

uint32_t system_timer_chan_1_was_interrupted() {
	return *SYSTEM_TIMER_CONTROL_STATUS & 2;
}
