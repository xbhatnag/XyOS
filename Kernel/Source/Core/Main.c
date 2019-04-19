#include "Exceptions.h"
#include "UART.h"
#include "ConsoleIO.h"
#include "ARMTimer.h"
#include "SystemTimer.h"
#include "InterruptController.h"
#include "Mailbox.h"
#include "TrafficLights.h"
#include "ThreadHandling.h"
#include <stdint.h>

// Implemented in MainAssembly.S
extern uint64_t exception_level();

// Implemented in ExceptionsAssembly.S
extern void enable_irq();

// Implemented in VectorTable.S
extern void set_vector_table();

extern uint64_t get_stack_ptr();

void main() {
	// Normally we would setup UART here
	// but that is not necessary because
	// the SerialClient or Bootloader
	// already did it.
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
	println("°C");

	print("┣━ ARM Memory Size : ");
	puti_32(mailbox_get_arm_memory_size() / (1024 * 1024));
	println(" MB");

	print("┣━ VC Memory Size : ");
	puti_32(mailbox_get_vc_memory_size() / (1024 * 1024));
	println(" MB");

	print("┗━ Current Exception Level : ");
	putc(exception_level() + '0');
	newline();

	if (exception_level() != 1) {
		error("Unknown exception level");
	}

	newline();

	println("## Welcome to EL1");

	println("┣━ Initializing Traffic Lights");
	traffic_lights_init();

	println("┣━ Enabling IRQ");
	enable_irq();

	println("┣━ Enabling ARM Timer");
	arm_timer_enable();

	println("┣━ Enabling ARM Timer IRQ");
	ic_enable_arm_timer_irq();

	println("┣━ Enabling System Timer IRQ");
	ic_enable_system_timer_chan_1_irq();

	println("┗━ Initializing Kernel Threads");
	newline();

	initialize_kernel_threads();

	error("We should never reach here");
}
