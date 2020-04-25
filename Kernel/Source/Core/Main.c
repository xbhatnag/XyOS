#include "Exceptions.h"
#include "UART.h"
#include "ConsoleIO.h"
#include "ARMTimer.h"
#include "SystemTimer.h"
#include "InterruptController.h"
#include "Mailbox.h"
#include "Menu.h"
#include "FrameBuffer.h"
#include "FrameBufferGraphics.h"
#include "TrafficLights.h"
#include "Threading.h"
#include "VirtualMemory.h"
#include <stdint.h>

// Implemented in MainAssembly.S
extern uint64_t exception_level();

// Implemented in ExceptionsAssembly.S
extern void irq_enable();

// Implemented in VectorTable.S
extern void set_vector_table();

void main() {
	// Normally we would setup UART here
	// but that is not necessary because
	// the SerialClient or Bootloader
	// already did it.

	// TODO: This is a hack. Fix this!
	is_frame_buffer_ready = 0;

	println("## Booting XyOS");

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

	print("┣━ Current Exception Level : ");
	putc(exception_level() + '0');
	newline();

	if (exception_level() != 1) {
		error("Unknown exception level");
	}

	println("┣━ Initializing Traffic Lights");
	traffic_lights_init();

	println("┣━ Enabling IRQ");
	irq_enable();

	println("┣━ Enabling ARM Timer");
	arm_timer_enable();

	println("┣━ Enabling ARM Timer IRQ");
	ic_enable_arm_timer_irq();

	println("┣━ Enabling System Timer IRQ");
	ic_enable_system_timer_chan_1_irq();

	println("┣━ Initializing Virtual Memory");
	vmem_init();

	println("┣━ Initializing Frame Buffer");
	frame_buffer_init(1024, 768);
	frame_buffer_clear();

	thread_0();

	error("We should never reach here");
}
