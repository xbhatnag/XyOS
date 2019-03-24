#include "FrameBuffer.h"
#include "Mailbox.h"
#include "ConsoleIO.h"
#include "Exceptions.h"

void print_hex_8(char value) {
	uint32_t hex1 = value >> 4;
	if (hex1 >= 10) {
		putc(hex1 - 10 + 'a');
	} else {
		putc(hex1 + '0');
	}
	uint32_t hex2 = value & 0xf;
	if (hex2 >= 10) {
		putc(hex2 - 10 + 'a');
	} else {
		putc(hex2 + '0');
	}
}

void frame_buffer_allocate() {
	uint32_t __attribute__((aligned(16))) message[8];
	// message header
	message[0] = sizeof(message);
	message[1] = 0;

	// tag 1 header
	message[2] = 0x00040001;
	message[3] = 8;
	message[4] = 0;

	// tag 1 data
	message[5] = 8;		// alignment in bytes / frame buffer base address in bytes
	message[6] = 0;		// frame buffer size in bytes

	// end tag
	message[7] = 0;

	// write to mailbox
	mailbox_write(&message[0]);

	// wait until read is possible (why?)
	mailbox_wait_read();

	if (message[1] != 0x80000000) {
		error("Could not allocate buffer.");
	}

	print("Frame Buffer Base Address = ");
	puti_32(message[5]);
	newline();

	print("Frame Buffer Size in Bytes = ");
	puti_32(message[6]);
	newline();
}

uint32_t frame_buffer_get_edid_block(uint32_t block_num) {
	uint32_t __attribute__((aligned(16))) message[40];
	// message header
	message[0] = sizeof(message);
	message[1] = 0;

	// tag 1 header
	message[2] = 0x00030020;
	message[3] = 136;
	message[4] = 0;

	// tag 1 data
	message[5] = block_num;		// block number
	message[6] = 0;		        // status

	// end tag
	message[39] = 0;

	// write to mailbox
	mailbox_write(&message[0]);

	// wait until read is possible
	mailbox_wait_read();

	if (message[1] != 0x80000000) {
		error("Could not get EDID block.");
	}

	print("EDID Block Status = ");
	puti_32(message[6]);
	newline();

	if (message[6] > 0) {
		return 0;
	}

	char* edid = (char*) &message[7];
	for (uint32_t i = 1; i <= 128; i++) {
		print_hex_8(*edid);
		putc(' ');
		if (i % 32 == 0) {
			newline();
		}
		edid += 1;
	}
	return 1;
}

void frame_buffer_blank_screen() {
	uint32_t __attribute__((aligned(16))) message[7];
	// message header
	message[0] = sizeof(message);
	message[1] = 0;

	// tag 1 header
	message[2] = 0x00040002;
	message[3] = 4;
	message[4] = 0;

	// tag 1 data
	message[5] = 1;		// state

	// end tag
	message[6] = 0;

	// write to mailbox
	mailbox_write(&message[0]);

	// wait until read is possible
	mailbox_wait_read();

	if (message[1] != 0x80000000) {
		error("Could not blank screen.");
	}
}
