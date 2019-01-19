#include "mmap_io.h"
#include "timer.h"

#define TIMER_BASE		 (MMAP_IO_BASE + 0xB400)
#define TIMER_LOAD		 ((uint32_t *)(TIMER_BASE))
#define TIMER_VALUE		 ((uint32_t *)(TIMER_BASE + 0x4))
#define TIMER_CONTROL		 ((uint32_t *)(TIMER_BASE + 0x8))
#define TIMER_IRQ_ACK		 ((uint32_t *)(TIMER_BASE + 0xC))
#define TIMER_RAW_IRQ		 ((uint32_t *)(TIMER_BASE + 0x10))
#define TIMER_MASK_IRQ		 ((uint32_t *)(TIMER_BASE + 0x14))

void timer_enable() {
	*TIMER_CONTROL = 0x3E00AA;
	*TIMER_IRQ_ACK = 0;
}

uint32_t timer_is_enabled() {
	uint32_t val = *TIMER_CONTROL;
	if (val == 0x3E00AA) {
		return 1;
	} else {
		return 0;
	}
}

uint32_t timer_interrupted() {
	return *TIMER_MASK_IRQ;
}

uint32_t timer_get_value() {
	return *TIMER_VALUE;
}

void timer_countdown(uint32_t miliseconds) {
	*TIMER_LOAD = miliseconds;
}
