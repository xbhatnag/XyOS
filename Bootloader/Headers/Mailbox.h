#ifndef _MAILBOX_H_
#define _MAILBOX_H_

#include "Peripherals.h"
#include <stdint.h>

#define MAILBOX_BASE        (PERIPHERALS_BASE + 0xB880)
#define MAILBOX_READ        ((volatile uint32_t*)(MAILBOX_BASE))
#define MAILBOX_STATUS      ((volatile uint32_t*)(MAILBOX_BASE + 0x18))
#define MAILBOX_WRITE      	((volatile uint32_t*)(MAILBOX_BASE + 0x20))

uint32_t mailbox_get_arm_memory_size();

#endif
