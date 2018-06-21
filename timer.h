#ifndef _TIMER_H_
#define _TIMER_H_

void timer_enable();
int timer_isEnabled();
int timer_interrupted();
int timer_getValue();
void countdown(int seconds);
#endif
