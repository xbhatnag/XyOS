#include "gpio.h"
#include "error.h"
#include "uart.h"
#include "stdio.h"
#include "arm_timer.h"
#include "system_timer.h"
#include "interrupts.h"
#include "mailbox.h"
#include <stdint.h>

// Implemented in interrupts_asm.S
extern void enable_irq();

// Implemented in exceptions_asm.S
extern int exception_level();
extern void jump_to_el1();

// Implemented in vectors_asm.S
extern void set_vector_table();

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
	println("## Welcome to EL1");
	println("┣━ Enabling IRQ");
	enable_irq();

	println("┣━ Enabling ARM Timer");
	arm_timer_enable();

	println("┣━ Enabling ARM Timer IRQ");
	ic_enable_arm_timer_irq();

	println("┗━ Enabling System Timer IRQ");
	ic_enable_system_timer_chan_1_irq();

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
	*((volatile uint32_t*)0x3);
	println("Return from exception");
}

void start_clock() {
	println("Starting clock");
	system_timer_chan_1_countdown(1000);
}

void hypervisor_transfer() {
	println("Transferring to hypervisor");
	asm("hvc #69");
	println("Returned from hypervisor");
}

void typewriter_mode() {
	println("Typewriter Mode On");
	while(1) {
		char input = getc();
		if (input == 3) {
			newline();
			return;
		} else {
			putc(input);
		}
	}
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
		case 't': {
			typewriter_mode();
			break;
		}
		case 'y': {
			println("Wait says the yellow one,");
			println("Blinking in between");
			gpio_set(10, GPIO_LEVEL_HIGH);
			break;
		}
		case 'l': {
			println("Turning LED on...");
			mailbox_turn_led_on();
			break;
		}
		case 'c': {
			start_clock();
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

	print("Firmware Revision : ");
	puti_32(mailbox_get_firmware_revision());
	newline();

	print("Board Model : ");
	puti_32(mailbox_get_board_model());
	newline();

	print("Board Revision : ");
	puti_32(mailbox_get_board_revision());
	newline();

	print("Current Exception Level : ");
	putc(exception_level() + '0');
	newline();

	print("ARM Memory Size : ");
	puti_32(mailbox_get_arm_memory_size());
	print(" MB");
	newline();

	print("VC Memory Size : ");
	puti_32(mailbox_get_vc_memory_size());
	print(" MB");
	newline();

	switch (exception_level()) {
		case 2: el2_main(); break;
		case 1: el1_main(); break;
		default: error("Unknown exception level");
	}

	error("Returned from main menu");
}
