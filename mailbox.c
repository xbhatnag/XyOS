#include "mailbox.h"
#include "error.h"

extern void mailbox_write(uint32_t* address);
extern void mailbox_wait_read();

void mailbox_turn_led_on() {
	uint32_t __attribute__((aligned(16))) message[8];
	// message header
	message[0] = sizeof(message);
	message[1] = 0;

	// tag 1 header
	message[2] = 0x00038041;
	message[3] = 8;
	message[4] = 0;

	// tag 1 data
	message[5] = 130;
	message[6] = 1;

	// end tag
	message[7] = 0;
	
	// write to mailbox
	mailbox_write(&message[0]);

	// wait until read is possible (why?)
	mailbox_wait_read();

	if (message[1] != 0x80000000) {
		error("Could not turn LED on. Mailbox write failed.");
	}
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

	return message[6] / (1000 * 1000);
}

uint32_t mailbox_get_vc_memory_size() {
	uint32_t __attribute__((aligned(16))) message[8];
	// message header
	message[0] = sizeof(message);	// Message Size
	message[1] = 0;			// Request Code

	// tag 1 header
	message[2] = 0x00010006;	// Get VC memory
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
		error("Could not get VC RAM. Mailbox write failed.");
	}

	return message[6] / (1000 * 1000);
}

uint32_t mailbox_get_firmware_revision() {
	uint32_t __attribute__((aligned(16))) message[7];
	// message header
	message[0] = sizeof(message);	// Message Size
	message[1] = 0;			// Request Code

	// tag 1 header
	message[2] = 0x00000001;	// Get firmware revision
	message[3] = 4;			// 4 bytes in response
	message[4] = 0;			// Request Code

	// tag 1 data
	message[5] = 0;			// Firmware revision

	// end tag
	message[6] = 0;

	// write to mailbox	
	mailbox_write(&message[0]);

	// wait until read is possible
	mailbox_wait_read();
	
	if (message[1] != 0x80000000) {
		error("Could not get Firmware Revision. Mailbox write failed.");
	}

	return message[5];
}

uint32_t mailbox_get_board_model() {
	uint32_t __attribute__((aligned(16))) message[7];
	// message header
	message[0] = sizeof(message);	// Message Size
	message[1] = 0;			// Request Code

	// tag 1 header
	message[2] = 0x00010001;	// Get board model
	message[3] = 4;			// 4 bytes in response
	message[4] = 0;			// Request Code

	// tag 1 data
	message[5] = 0;			// Board model

	// end tag
	message[6] = 0;

	// write to mailbox	
	mailbox_write(&message[0]);

	// wait until read is possible
	mailbox_wait_read();

	if (message[1] != 0x80000000) {
		error("Could not get Board Model. Mailbox write failed.");
	}

	return message[5];
}

uint32_t mailbox_get_board_revision() {
	uint32_t __attribute__((aligned(16))) message[7];
	// message header
	message[0] = sizeof(message);	// Message Size
	message[1] = 0;			// Request Code

	// tag 1 header
	message[2] = 0x00010002;	// Get board revision
	message[3] = 4;			// 4 bytes in response
	message[4] = 0;			// Request Code

	// tag 1 data
	message[5] = 0;			// Board revision

	// end tag
	message[6] = 0;

	// write to mailbox	
	mailbox_write(&message[0]);

	// wait until read is possible
	mailbox_wait_read();

	if (message[1] != 0x80000000) {
		error("Could not get Board Revision. Mailbox write failed.");
	}

	return message[5];
}

