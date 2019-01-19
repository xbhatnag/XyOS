#include "error.h"
#include "stdio.h"

// Exception level 2 handling
extern uint64_t exception_syndrome_2();
extern uint64_t exception_link_2();
extern uint64_t saved_program_state_2();

// Exception level 1 handling
extern uint64_t exception_syndrome_1();
extern uint64_t exception_link_1();
extern uint64_t saved_program_state_1();
extern void increment_elr_1();


void exception_handler_2() {
	println("######### EXCEPTION #########");
	println("Exception Level : 2");

	// Exception Syndrome
	uint64_t esr = exception_syndrome_2();
	print("Exception Syndrome (Bits) : ");
	pretty_putb_32(esr);
	newline();

	// Exception Link
	uint64_t elr = exception_link_1();
	print("Exception Link (Bits) : ");
	pretty_putb_32(elr);
	newline();

	// Saved Program State
	uint64_t sps = saved_program_state_1();
	print("Saved Program State (Bits) : ");
	pretty_putb_32(sps);
	newline();

	println("#############################");
}

void exception_handler_1() {
	// Move to next instruction
	increment_elr_1();

	// Print exception details
	println("######### EXCEPTION #########");
	println("Exception Level : 1");

	// Exception Syndrome
	uint64_t esr = exception_syndrome_1();
	print("Exception Syndrome (Bits) : ");
	pretty_putb_32(esr);
	newline();

	// Exception Link
	uint64_t elr = exception_link_1();
	print("Exception Link (Bits) : ");
	pretty_putb_32(elr);
	newline();

	// Saved Program State
	uint64_t sps = saved_program_state_1();
	print("Saved Program State (Bits) : ");
	pretty_putb_32(sps);
	newline();
	
	println("#############################");
}
