#include "stdio.h"
#include <stdint.h>

extern void clear_all_daif();
extern void set_vector_table();
extern void set_secure_config();
extern uint64_t exception_syndrome();
extern int instruction_length();
extern uint64_t fault_address();

void print_bits(uint64_t num, int end, int start) {
	for (int i = end; i >= start; i--) {
		int bit = ((num >> i) & 0x1);
		if (bit) {
			putc('1');
		} else {
			putc('0');
		}
	}
}

void enable_interrupts(){
	println("Setting up Vector Table...");
	set_vector_table();
	println("Setting up Hypervisor Secure Config...");
	set_secure_config();
	println("Clearing all DAIF bits...");
	clear_all_daif();
}

char* exception_class_str() {
	uint64_t esr = exception_syndrome();
	int ec = esr >> 26;
	switch(ec) {
		case 0x0: return "Unknown reason";
		case 0xe: return "Illegal execution state";
		case 0x20: return "Instruction abort from lower exception level";
		case 0x21: return "Instruction abort from same exception level";
		case 0x22: return "PC Alignment fault";
		case 0x24: return "Data abort from lower exception level";
		case 0x25: return "Data abort from same exception level";
		case 0x26: return "SP alignment fault";
		case 0x28: return "Floating point exception in 32-Bit mode";
		case 0x2c: return "Floating point exception in 64-Bit mode";
		case 0x2f: return "SError interrupt";
		default: return "Some other kind I haven't described";
	}
}

char* instruction_length_str() {
	if (instruction_length()) {
		return "32-Bit";
	}
	return "16-Bit";
}

void exception_handler() {
	println("######### EXCEPTION #########");

	// Exception Syndrome
	printf("Exception Syndrome : ");
	uint64_t esr = exception_syndrome();
	print_bits(esr, 31, 0);
	newline();
	
	// Exception Class
	printf("Exception Class : ");
	println(exception_class_str());

	// Instruction Length
	printf("Instruction Length : ");
	println(instruction_length_str());

	// Fault Address
	uint64_t fault_addr = fault_address();
	printf("Instruction Fault Address : ");
	print_bits(fault_addr, 63, 32);
	newline();
	printf("Data Fault Address : ");
	print_bits(fault_addr, 31, 0);
	newline();

	println("#############################");

	while(1) {
	}
}
