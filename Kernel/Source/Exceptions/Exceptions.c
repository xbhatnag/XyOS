#include "Exceptions.h"
#include "ConsoleIO.h"
#include "Mailbox.h"
#include "ARMTimer.h"
#include "SystemTimer.h"
#include "Threading.h"

// Exception level 2 handling
extern uint64_t exception_syndrome_2();
extern uint64_t exception_link_2();
extern uint64_t saved_program_state_2();

// Exception level 1 handling
extern uint64_t exception_syndrome_1();
extern uint64_t exception_link_1();
extern uint64_t saved_program_state_1();
extern uint64_t interrupt_status_1();

void exception_details_1() {
	// Print exception details
	println("######### EXCEPTION #########");
	println("Exception Level\t\t\t1");

	// Interrupt Status
	uint64_t isr = interrupt_status_1();
	print("Interrupt Status (Bits)\t\t");
	pretty_putb_64(isr);
	newline();

	puth_with_title_64("Interrupt Status (Hex)\t\t", isr);

	// Exception Syndrome
	uint64_t esr = exception_syndrome_1();
	print("Exception Syndrome (Bits)\t");
	pretty_putb_64(esr);
	newline();

	puth_with_title_64("Exception Syndrome (Hex)\t", esr);

	// Exception Link
	uint64_t elr = exception_link_1();
	print("Exception Link (Bits)\t\t");
	pretty_putb_64(elr);
	newline();

	puth_with_title_64("Exception Link (Hex)\t\t", elr);

	// Saved Program State
	uint64_t sps = saved_program_state_1();
	print("Saved Program State (Bits)\t");
	pretty_putb_64(sps);
	newline();

	puth_with_title_64("Saved Program State (Hex)\t", sps);

	println("#############################");
}

void exception_handler_1() {
	// Attempt to process exception
	if (interrupt_status_1()) {
		// Looks to be a H/W interrupt
		if (arm_timer_was_interrupted()) {
			// ARM timer corresponds to scheduler work
			thread_switch();
    		arm_timer_ack_interrupt();
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
