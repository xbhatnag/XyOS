#include "ARMTimer.h"
#include "UART.h"
#include "Exceptions.h"
#include "ConsoleIO.h"
#include "Mailbox.h"
#include "Threading.h"
#include "FrameBuffer.h"
#include "FrameBufferGraphics.h"
#include "VirtualMemory.h"
#include "Menu.h"

struct Thread {
    uintptr_t saved_program_counter;
    uintptr_t saved_stack_pointer;
} threads[NUMBER_OF_THREADS];

volatile uintptr_t current_thread_saved_program_counter = 0;
volatile uintptr_t current_thread_saved_stack_pointer = 0;
volatile uint64_t current_thread_num = 0;
volatile uint8_t is_threading_ready = 0;
volatile uint64_t counter = 0;

void thread_0() {
    main_menu();
}

void thread_1() {
    while(1) {
        counter+= 1;
    }
}

void thread_2() {
    while(1) {
        volatile uint64_t counter_seen = counter;
        frame_buffer_puti_64(counter_seen);
    }
}

void thread_3() {
    while(1) {
        frame_buffer_draw_square_corner(0xFF0000);
        frame_buffer_draw_square_corner(0x00FF00);
        frame_buffer_draw_square_corner(0x0000FF);
        frame_buffer_draw_square_corner(0xFFFFFF);
        frame_buffer_draw_square_corner(0x000000);
    }
}

void threading_init() {
    // TODO: Figure out why starting the stack at the end of the page messes things up.
    // Allocate stack space for 3 threads
    threads[0].saved_program_counter = (uintptr_t) &thread_0;
    threads[0].saved_stack_pointer = allocate_from_lower_l3(0x10000000, PAGE_SIZE) + 0x9ff0;

    threads[1].saved_program_counter = (uintptr_t) &thread_1;
    threads[1].saved_stack_pointer = allocate_from_lower_l3(0x10010000, PAGE_SIZE) + 0x9ff0;

    // threads[2].saved_program_counter = (uintptr_t) &thread_2;
    // threads[2].saved_stack_pointer = allocate_from_lower_l3(0x10020000, PAGE_SIZE) + 0x9ff0;

    // threads[3].saved_program_counter = (uintptr_t) &thread_3;
    // threads[3].saved_stack_pointer = allocate_from_lower_l3(0x10030000, PAGE_SIZE) + 0x9ff0;

    // Start up the scheduler
    arm_timer_countdown(SCHEDULER_FREQUENCY);

    // We never want to escape from here.
    // The idea is that when the ARM timer fires, this entire stack is getting replaced
    // by the scheduler, who will use it for thread switching.
    // So do nothing until that happens.
    while (1) {}
}

void thread_switch() {
    if (is_threading_ready) {
        // Save current stack pointer and program counter
        threads[current_thread_num].saved_stack_pointer = current_thread_saved_stack_pointer;
        threads[current_thread_num].saved_program_counter = current_thread_saved_program_counter;
    } else {
        // We came directly from the old stack.
        // Don't attempt to save it.
        // We don't want it anymore, because it will be replaced by the scheduler stack.
        is_threading_ready = 1;
    }

    // Move to next thread (round-robin)
    current_thread_num = (current_thread_num + 1) % NUMBER_OF_THREADS;

    // Load new stack pointer and program counter
    current_thread_saved_stack_pointer = threads[current_thread_num].saved_stack_pointer;
    current_thread_saved_program_counter = threads[current_thread_num].saved_program_counter;
}
