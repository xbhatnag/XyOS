#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdint.h>
#include "Peripherals.h"

// Addresses
#define GPIO_BASE           (PERIPHERALS_BASE + 0x200000)
#define GPIO_SEL0           ((volatile uint32_t*)(GPIO_BASE))
#define GPIO_SEL1           ((volatile uint32_t*)(GPIO_BASE + 0x4))
#define GPIO_SEL2           ((volatile uint32_t*)(GPIO_BASE + 0x8))
#define GPIO_SEL3           ((volatile uint32_t*)(GPIO_BASE + 0xC))
#define GPIO_SEL4           ((volatile uint32_t*)(GPIO_BASE + 0x10))
#define GPIO_SEL5           ((volatile uint32_t*)(GPIO_BASE + 0x14))
#define GPIO_SET0           ((volatile uint32_t*)(GPIO_BASE + 0x1C))
#define GPIO_SET1           ((volatile uint32_t*)(GPIO_BASE + 0x20))
#define GPIO_CLR0           ((volatile uint32_t*)(GPIO_BASE + 0x28))
#define GPIO_CLR1           ((volatile uint32_t*)(GPIO_BASE + 0x2C))
#define GPIO_LEV0           ((volatile uint32_t*)(GPIO_BASE + 0x34))
#define GPIO_LEV1           ((volatile uint32_t*)(GPIO_BASE + 0x38))
#define GPIO_PUD            ((volatile uint32_t*)(GPIO_BASE + 0x94))
#define GPIO_PUD_CLK0       ((volatile uint32_t*)(GPIO_BASE + 0x98))

// Modes
#define GPIO_MODE_INPUT     0x0
#define GPIO_MODE_OUTPUT    0x1
#define GPIO_MODE_ALT5      0x2

// Levels
#define GPIO_LEVEL_HIGH     0x1
#define GPIO_LEVEL_LOW      0x0

void gpio_mode(int pin, int mode);
void gpio_clear(int pin);
void gpio_set(int pin, int level);
#endif
