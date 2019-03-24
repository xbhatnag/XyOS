#include "GPIO.h"
#include "Exceptions.h"
#include "UART.h"
#include "ConsoleIO.h"
#include "ARMTimer.h"
#include "SystemTimer.h"
#include "InterruptController.h"
#include "Mailbox.h"
#include "FrameBuffer.h"
#include <stdint.h>

// Implemented in MainAssembly.S
extern uint64_t exception_level();

// Implemented in ExceptionsAssembly.S
extern void enable_irq();

// Implemented in VectorTable.S
extern void set_vector_table();

void force_exception(){
	println("Forcing unaligned data access...");
	*((volatile uint32_t*)0x3);
	println("Return from exception");
}

void start_stopwatch() {
	println("Starting stopwatch");
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
		case 'a': {
			uint64_t* address = frame_buffer_allocate();
			for (uint64_t i = 0; i < 20; i++) {
				*address = 0xFFFFFFFFFFFFFFFF;
			}
			break;
		}
		case 'b': {
			uint32_t block_num = 0;
			while (frame_buffer_get_edid_block(block_num)) {
				block_num += 1;
				newline();
				newline();
			}
			break;
		}
		case 'd': {
			frame_buffer_blank_screen();
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
		case 's': {
			start_stopwatch();
			break;
		}
		case 'c': {
			mailbox_print_all_clocks();
			break;
		}
		default: {
			println("Unknown command");
		}
	}
}

void main() {
	// Setup UART1
	//uart_setup();

	// Setup GPIO Traffic Lights
	gpio_mode(9, GPIO_MODE_OUTPUT);
	gpio_mode(10, GPIO_MODE_OUTPUT);
	gpio_mode(11, GPIO_MODE_OUTPUT);

	println("## Welcome to XyOS");
	println("┣━ Configuring Vector Table");
	set_vector_table();

	print("┣━ Firmware Revision : ");
	puti_32(mailbox_get_firmware_revision());
	newline();

	print("┣━ Board Model : ");
	puti_32(mailbox_get_board_model());
	newline();

	print("┣━ Board Revision : ");
	puti_32(mailbox_get_board_revision());
	newline();

	print("┣━ System Temperature : ");
	puti_32(mailbox_get_system_temp());
	print("°C");
	newline();

	print("┣━ ARM Memory Size : ");
	puti_32(mailbox_get_arm_memory_size() / (1024 * 1024));
	print(" MB");
	newline();

	print("┣━ VC Memory Size : ");
	puti_32(mailbox_get_vc_memory_size() / (1024 * 1024));
	print(" MB");
	newline();

	print("┗━ Current Exception Level : ");
	putc(exception_level() + '0');
	newline();

	if (exception_level() != 1) {
		error("Unknown exception level");
	}

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

	error("Returned from main menu");
}
