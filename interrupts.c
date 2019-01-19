#include "mmap_io.h"
#include "interrupts.h"

#define IC_BASE				(MMAP_IO_BASE + 0xB200)
#define IC_IRQ_BASIC_PENDING		((uint32_t *)(IC_BASE))
#define IC_IRQ_PENDING_1		((uint32_t *)(IC_BASE + 0x4))
#define IC_IRQ_PENDING_2		((uint32_t *)(IC_BASE + 0x8))
#define IC_FIQ_CONTROL			((uint32_t *)(IC_BASE + 0xC))
#define IC_ENABLE_IRQ_1			((uint32_t *)(IC_BASE + 0x10))
#define IC_ENABLE_IRQ_2			((uint32_t *)(IC_BASE + 0x14))
#define IC_ENABLE_BASIC_IRQ		((uint32_t *)(IC_BASE + 0x18))
#define IC_DISABLE_IRQ_1		((uint32_t *)(IC_BASE + 0x1C))
#define IC_DISABLE_IRQ_2		((uint32_t *)(IC_BASE + 0x20))
#define IC_DISABLE_BASIC_IRQ		((uint32_t *)(IC_BASE + 0x24))

void ic_enable_timer_irq() {
	*IC_ENABLE_BASIC_IRQ |= 1;	
}

uint32_t ic_basic_irq_pending() {
	return *IC_IRQ_BASIC_PENDING;
}
