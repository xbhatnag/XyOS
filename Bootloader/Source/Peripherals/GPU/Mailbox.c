#include "Mailbox.h"
#include "ConsoleIO.h"
#include "Exceptions.h"

void mailbox_write(uint32_t* address) {
	uintptr_t value = (uintptr_t) address;
	while (*MAILBOX_STATUS & 0x80000000) {} // Wait while full flag set
	value |= 0x8;	// Channel 8
	*MAILBOX_WRITE = value;
}

void mailbox_wait_read() {
	while (*MAILBOX_STATUS & 0x40000000) {} // Wait while empty flag set
	while (!(*MAILBOX_READ & 0x8)) {} // Wait while not channel 8
}

uint32_t mailbox_get_arm_memory_size() {
	uint32_t __attribute__((aligned(16))) message[8];
	// message header
	message[0] = sizeof(message);	// Message Size
	message[1] = 0;			// Request Code

	// tag 1 header
	message[2] = 0x00010005;	// Get ARM memory
	message[3] = 8;			// 8 bytes in response
	message[4] = 0;			// Request Code

	// tag 1 data
	message[5] = 0;			// Offset of memory
	message[6] = 0;			// Memory size in bytes

	// end tag
	message[7] = 0;

	// write to mailbox
	mailbox_write(&message[0]);

	// wait until read is possible
	mailbox_wait_read();

	if (message[1] != 0x80000000 || message[4] != 0x80000008) {
		error("Could not get ARM RAM. Mailbox write failed.");
	}

	if (message[5] != 0) {
		error("ARM RAM offset is not 0.");
	}

	return message[6];
}
