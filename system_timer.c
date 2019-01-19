#include "mmap_io.h"
#include "system_timer.h"

#define SYSTEM_TIMER_BASE		(MMAP_IO_BASE + 0x3000)
#define SYSTEM_TIMER_CONTROL_STATUS	((uint32_t *)(SYSTEM_TIMER_BASE))
#define SYSTEM_TIMER_COUNTER		((uint64_t *)(SYSTEM_TIMER_BASE + 0x4))
#define SYSTEM_TIMER_COUNTER_L32	((uint32_t *)(SYSTEM_TIMER_BASE + 0x4))
#define SYSTEM_TIMER_COUNTER_U32	((uint32_t *)(SYSTEM_TIMER_BASE + 0x8))
#define SYSTEM_TIMER_COMPARE_0		((uint32_t *)(SYSTEM_TIMER_BASE + 0xC))
#define SYSTEM_TIMER_COMPARE_1		((uint32_t *)(SYSTEM_TIMER_BASE + 0x10))
#define SYSTEM_TIMER_COMPARE_2		((uint32_t *)(SYSTEM_TIMER_BASE + 0x14))
#define SYSTEM_TIMER_COMPARE_3		((uint32_t *)(SYSTEM_TIMER_BASE + 0x18))

void system_timer_chan_1_countdown(uint32_t miliseconds) {
	*SYSTEM_TIMER_COMPARE_1 = ((*SYSTEM_TIMER_COUNTER_L32) + (1000 * miliseconds));
}

void system_timer_chan_1_ack_interrupt() {
	*SYSTEM_TIMER_CONTROL_STATUS = 2;
}

uint32_t system_timer_chan_1_was_interrupted() {
	return *SYSTEM_TIMER_CONTROL_STATUS & 2;
}
