#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

void timer_enable();
uint32_t timer_is_enabled();
uint32_t timer_interrupted();
uint32_t timer_get_value();
void timer_countdown(uint32_t seconds);
#endif
