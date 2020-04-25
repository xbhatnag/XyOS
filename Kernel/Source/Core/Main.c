#include "Exceptions.h"
#include "ConsoleIO.h"
#include "ARMTimer.h"
#include "InterruptController.h"
#include "Mailbox.h"
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
	println("# Booting XyOS");

	println("┣━ Configuring Vector Table");
	set_vector_table();

	printf("┣━ Firmware Revision : %u\n", mailbox_get_firmware_revision());
	printf("┣━ Board Model : %u\n", mailbox_get_board_model());
	printf("┣━ Board Revision : %u\n", mailbox_get_board_revision());
	printf("┣━ System Temperature : %u°C\n", mailbox_get_system_temp());
	printf("┣━ ARM Memory Size : %u MB\n", mailbox_get_arm_memory_size() / (1024 * 1024));
	printf("┣━ VC Memory Size : %u MB\n", mailbox_get_vc_memory_size() / (1024 * 1024));
	printf("┣━ Current Exception Level : %u\n", exception_level());

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

	println("┗━ Initializing Virtual Memory");
	vmem_init();

	//println("┣━ Initializing Frame Buffer");
	//frame_buffer_init(1024, 768);
	//frame_buffer_clear();

	thread_0();

	error("We should never reach here");
}
