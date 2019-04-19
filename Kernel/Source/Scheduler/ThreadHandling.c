#include "ARMTimer.h"
#include "UART.h"
#include "Exceptions.h"
#include "ConsoleIO.h"
#include "Mailbox.h"
#include "ThreadHandling.h"
#include "Menu.h"

struct Thread {
    uintptr_t saved_program_counter;
    uintptr_t saved_stack_pointer;
} threads[NUMBER_OF_THREADS];

uintptr_t current_thread_saved_program_counter = 0;
uintptr_t current_thread_saved_stack_pointer = 0;
uint64_t current_thread_num = 0;
volatile uint64_t counter = 0;

void thread_0() {
    main_menu();
}

void thread_1() {
    while(1) {
        counter++;
    }
}

void initialize_kernel_threads() {
    // Create Thread 1
    threads[1].saved_program_counter = (uintptr_t) &thread_1;
    threads[1].saved_stack_pointer = 0xFFFF000000020000;

    // Start up the scheduler
    arm_timer_countdown(SCHEDULER_FREQUENCY);

    // Thread 0 is the main kernel thread.
    // The main kernel thread cannot be escaped.
    // It is always present and always doing something.
    // We segway into thread 0 here.
    thread_0();
    error("Thread 0 escaped!");
}

void thread_switch() {
    // Acknowledge timer interrupt
    arm_timer_ack_interrupt();

    // Save current stack pointer and program counter
    threads[current_thread_num].saved_stack_pointer = current_thread_saved_stack_pointer;
    threads[current_thread_num].saved_program_counter = current_thread_saved_program_counter;

    // Move to next thread (round-robin)
    current_thread_num = (current_thread_num + 1) % NUMBER_OF_THREADS;

    // Load new stack pointer and program counter
    current_thread_saved_stack_pointer = threads[current_thread_num].saved_stack_pointer;
    current_thread_saved_program_counter = threads[current_thread_num].saved_program_counter;
}
