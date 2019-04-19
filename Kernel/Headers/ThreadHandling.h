#ifndef __THREAD_HANDLING_H__
#define __THREAD_HANDLING_H__

#define SCHEDULER_FREQUENCY 1 // milisecond
#define NUMBER_OF_THREADS   2

extern volatile uint64_t counter;

void initialize_kernel_threads();
void thread_switch();

#endif
