#ifndef _INTERRUPT_CONTROLLER_H_
#define _INTERRUPT_CONTROLLER_H_

#include "Peripherals.h"
#include <stdint.h>

#define IC_BASE                     (PERIPHERALS_BASE + 0xB200)
#define IC_IRQ_BASIC_PENDING        ((volatile uint32_t *)(IC_BASE))
#define IC_IRQ_PENDING_1            ((volatile uint32_t *)(IC_BASE + 0x4))
#define IC_IRQ_PENDING_2            ((volatile uint32_t *)(IC_BASE + 0x8))
#define IC_FIQ_CONTROL              ((volatile uint32_t *)(IC_BASE + 0xC))
#define IC_ENABLE_IRQ_1             ((volatile uint32_t *)(IC_BASE + 0x10))
#define IC_ENABLE_IRQ_2             ((volatile uint32_t *)(IC_BASE + 0x14))
#define IC_ENABLE_BASIC_IRQ         ((volatile uint32_t *)(IC_BASE + 0x18))
#define IC_DISABLE_IRQ_1            ((volatile uint32_t *)(IC_BASE + 0x1C))
#define IC_DISABLE_IRQ_2            ((volatile uint32_t *)(IC_BASE + 0x20))
#define IC_DISABLE_BASIC_IRQ        ((volatile uint32_t *)(IC_BASE + 0x24))

void ic_enable_arm_timer_irq();
void ic_enable_system_timer_chan_1_irq();
uint32_t ic_basic_irq_pending();

#endif
