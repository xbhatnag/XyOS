#include "Exceptions.h"
#include "ConsoleIO.h"
#include "ARMTimer.h"
#include "SystemTimer.h"

// Exception level 2 handling
extern uint64_t exception_syndrome_2();
extern uint64_t exception_link_2();
extern uint64_t saved_program_state_2();

// Exception level 1 handling
extern uint64_t exception_syndrome_1();
extern uint64_t exception_link_1();
extern uint64_t saved_program_state_1();
extern uint64_t interrupt_status_1();

uint64_t stopwatch = 0;

void exception_details_1() {
	// Print exception details
	println("######### EXCEPTION #########");
	println("Exception Level : 1");

	// Interrupt Status
	uint64_t isr = interrupt_status_1();
	print("Interrupt Status (Bits) : ");
	pretty_putb_64(isr);
	newline();

	// Exception Syndrome
	uint64_t esr = exception_syndrome_1();
	print("Exception Syndrome (Bits) : ");
	pretty_putb_64(esr);
	newline();

	// Exception Link
	uint64_t elr = exception_link_1();
	print("Exception Link (Bits) : ");
	pretty_putb_64(elr);
	newline();

	// Saved Program State
	uint64_t sps = saved_program_state_1();
	print("Saved Program State (Bits) : ");
	pretty_putb_64(sps);
	newline();

	println("#############################");
}

void tick() {
	// Increment the clock tick
	stopwatch += 1;

	// Save the cursor position, move cursor to top row, clear row
	print("\0337\033[H\033[2KSTOPWATCH: ");

	// Print the clock there
	puti_64(stopwatch);

	// Restore the cursor to its original position
	print("\0338");
}

void exception_handler_1() {
	// Attempt to process exception
	if (interrupt_status_1()) {
		// Looks to be a H/W interrupt
		if (arm_timer_was_interrupted()) {
			arm_timer_ack_interrupt();
		} else if (system_timer_chan_1_was_interrupted()) {
			// Treat system timer channel 1 as a clock for now
			tick();
			system_timer_chan_1_ack_interrupt();
			system_timer_chan_1_countdown(1000);
		} else {
			exception_details_1();
			error("Unknown H/W exception");
		}
	} else {
		// Looks to be a software interrupt
		exception_details_1();
		error("Unknown S/W exception");
	}
}

// Unrecoverable error has occurred
void error(char* msg) {
	println("########### ERROR ###########");
	println(msg);
	println("#############################");
	while(1) {}
}

void generic_error() {
	error("A generic error occurred");
}

// Convenience function for Assembly
void marker() {
	error("Marker was triggered!");
}
