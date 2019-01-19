#include "gpio.h"
#include "error.h"
#include "uart.h"
#include "stdio.h"
#include "timer.h"
#include "interrupts.h"

// Implemented in interrupts_asm.S
extern void enable_irq();

// Implemented in exceptions_asm.S
extern int exception_level();
extern void jump_to_el1();

// Implemented in vectors_asm.S
extern void set_vector_table();

extern uint64_t interrupt_status_1();

void menu(char c);

void el2_main() {
	newline();
	println("## Welcome to EL2");
	println("┣━ Setting up vector table");
	set_vector_table();

	println("┗━ Jumping to EL1");
	jump_to_el1();

	error("Jump to EL1 did not succeed");
}

void el1_main() {
	newline();
	println("## Welcome to EL2");
	println("┣━ Enabling IRQ");
	enable_irq();

	println("┣━ Enabling Timer");
	timer_enable();

	println("┗━ Enabling Timer Interrupts");
	ic_enable_timer_irq();

	newline();

	while(1) {
		// Wait for input!
		print("> ");
		char input = getc();
		putc(input);
		newline();
		menu(input);
	}
}

void force_exception(){
	println("Forcing unaligned data access...");
	*((volatile unsigned*)0x3);
	println("Return from exception");
}

void do_countdown() {
	if (timer_is_enabled()) {
		println("Timer is enabled");
	} else {
		println("Timer is not correctly enabled");
	}
	println("Starting countdown");
	timer_countdown(2000);
	while(timer_get_value()) {}
	for(unsigned i = 0; i < 10000; i++) { asm volatile ("nop");}
	if (timer_interrupted()) {
		println("Timer sent interrupt");
	} else {
		println("Timer did not send interrupt");
	}
	if (ic_basic_irq_pending()) {
		println("IC received timer interrupt");	
	} else {
		println("IC did not receive timer interrupt");
	}
}

void hypervisor_transfer() {
	println("Transferring to hypervisor");
	asm("hvc #69");
	println("Returned from hypervisor");
}

void status_interrupts() {
	uint64_t isr = interrupt_status_1();
	print("Interrupt Status (Bits) : ");
	pretty_putb_32(isr);
	newline();
}

void menu(char c){
	switch(c) {
		case 'e': {
			force_exception();
			break;
		}
		case 'r': {
			println("Stop says the red light");
			gpio_set(9, GPIO_LEVEL_HIGH);
			break;
		}
		case 'g': {
			println("Go says the green");
			gpio_set(11, GPIO_LEVEL_HIGH);
			break;
		}
		case 'h': {
			hypervisor_transfer();
			break;
		}

		case 's': {
			status_interrupts();
			break;
		}

		case 'y': {
			println("Wait says the yellow one,");
			println("Blinking in between");
			gpio_set(10, GPIO_LEVEL_HIGH);
			break;
		}
		case 't': {
			do_countdown();
			break;
		}
		default: {
			println("Unknown command");
		}
	}
}

void main() {
	// Setup UART1
	uart_setup();

	// Setup GPIO Traffic Lights
	gpio_mode(9, GPIO_MODE_OUTPUT);
	gpio_mode(10, GPIO_MODE_OUTPUT);
	gpio_mode(11, GPIO_MODE_OUTPUT);
	
	println("### Welcome to XyOS");
	print("Current Exception Level : ");
	putc(exception_level() + '0');
	newline();

	switch (exception_level()) {
		case 2: el2_main(); break;
		case 1: el1_main(); break;
		default: error("Unknown exception level");
	}

	error("Returned from main menu");
}


