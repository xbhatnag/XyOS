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
	println("Exception Level: 1");
	printf("Interrupt Status: %x\n", interrupt_status_1());
	printf("Exception Syndrome: %x\n", exception_syndrome_1());
	printf("Exception Link: %x\n", exception_link_1());
	printf("Saved Program State: %x\n", saved_program_state_1());
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
_Noreturn void error(char* msg) {
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
