#include "Exceptions.h"
#include "ConsoleIO.h"

// Exception level 2 handling
extern uint64_t exception_syndrome_2();
extern uint64_t exception_link_2();
extern uint64_t saved_program_state_2();

// Exception level 1 handling
extern uint64_t exception_syndrome_1();
extern uint64_t exception_link_1();
extern uint64_t saved_program_state_1();
extern uint64_t interrupt_status_1();

void exception_handler_2() {
	println("######### EXCEPTION #########");
	println("Exception Level : 2");

	// Exception Syndrome
	uint64_t esr = exception_syndrome_2();
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

	// No exception handling at EL2 yet
	error("Cannot handle exception");
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
