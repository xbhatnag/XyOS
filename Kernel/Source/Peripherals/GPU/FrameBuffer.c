#include "FrameBuffer.h"
#include "Mailbox.h"
#include "ConsoleIO.h"
#include "Exceptions.h"
#include "VirtualMemory.h"

extern uint64_t test_translation(uint64_t value);

void frame_buffer_init(uint32_t desired_width, uint32_t desired_height) {
	uint32_t desired_depth = 32;
	uint32_t desired_x_offset = 0;
	uint32_t desired_y_offset = 0;

	uint32_t __attribute__((aligned(16))) message[31];
	// message header
	message[0] = sizeof(message);
	message[1] = 0;

	// SET PHYSICAL WIDTH/HEIGHT
	// header
	message[2] = 0x00048003;
	message[3] = 8;
	message[4] = 0;

	// data
	message[5] = desired_width;
	message[6] = desired_height;

	// SET VIRTUAL WIDTH/HEIGHT
	// header
	message[7] = 0x00048004;
	message[8] = 8;
	message[9] = 0;

	// data
	message[10] = desired_width;
	message[11] = desired_height;

	// SET VIRTUAL OFFSET
	// header
	message[12] = 0x00048009;
	message[13] = 8;
	message[14] = 0;

	// data
	message[15] = desired_x_offset;
	message[16] = desired_y_offset;

	// SET DEPTH
	// header
	message[17] = 0x00048005;
	message[18] = 4;
	message[19] = 0;

	// data
	message[20] = desired_depth;	// bits per pixel

	// ALLOCATE THE BUFFER
	// header
	message[21] = 0x00040001;
	message[22] = 8;
	message[23] = 0;

	// data
	// TODO: This is necessary but why?
	//       Without it, some pixels at the end of the display are unavailable.
	message[24] = 0x10000;		// alignment in bytes / frame buffer base address in bytes
	message[25] = 0;			// frame buffer size in bytes

	// GET PITCH
	// header
	message[26] = 0x00040008;
	message[27] = 4;
	message[28] = 0;

	// data
	message[29] = 0;		// pitch

	// end tag
	message[30] = 0;

	// write to mailbox
	mailbox_write(&message[0]);

	// wait until read is possible
	mailbox_wait_read();

	// Make sure no errors occurred!
	if (message[1] != 0x80000000) {
		error("Errors found!");
	}

	// Set up the globals
	frame_buffer_width = message[10];
	frame_buffer_height = message[11];
	frame_buffer_physical_base_address = message[24] & FRAME_BUFFER_PADDR_MASK;
	frame_buffer_size = message[25];
	frame_buffer_pitch = message[29];
	frame_buffer_depth = message[20];

	uint32_t physical_width = message[5];
	uint32_t physical_height = message[6];
	uint32_t actual_x_offset = message[15];
	uint32_t actual_y_offset = message[16];

	if (desired_width != frame_buffer_width || desired_height != frame_buffer_height) {
		error("Width/Height are not what was desired");
	}

	// For now, physical and virtual dimensions are the same.
	if (physical_width != frame_buffer_width || physical_height != frame_buffer_height) {
		error("Physical and virtual dimensions are not the same!");
	}

	if (desired_depth != frame_buffer_depth) {
		error("Depth is not what was desired");
	}

	if (desired_x_offset != actual_x_offset || desired_y_offset != actual_y_offset) {
		error("Offsets are not what was desired");
	}

	if (frame_buffer_physical_base_address & 0xFFFF) {
		error("Frame buffer physical address misaligned");
	}

	uintptr_t base_address = allocate_from_lower_l3(frame_buffer_physical_base_address, frame_buffer_size);
	uintptr_t end_address = base_address + frame_buffer_size - 1;
	frame_buffer_virtual_base_address = (unsigned char*) base_address;


	println("   ╔══════════════════════════════════════════════════════════════╗");
	print("   ║ Frame Buffer Resolution = ");
	puti_32(frame_buffer_width);
	print(" x ");
	puti_32(frame_buffer_height);
	newline();

	puth_with_title_32("   ║ Frame Buffer Size = ", frame_buffer_size);
	puth_with_title_32("   ║ Frame Buffer Physical Base Address = ", frame_buffer_physical_base_address);
	puth_with_title_32("   ║ Frame Buffer Physical End Address = ", frame_buffer_physical_base_address + frame_buffer_size - 1);
	puth_with_title_64("   ║ Frame Buffer Virtual Base Address = ", base_address);
	puth_with_title_64("   ║ Frame Buffer Virtual End Address = ", end_address);
	puth_with_title_64("   ║ Test Translation (Virtual Base Address) = ", test_translation(base_address));
	puth_with_title_64("   ║ Test Translation (Virtual End Address) = ", test_translation(end_address));
	println("   ╚══════════════════════════════════════════════════════════════╝");

	// From this point on, all output is directed to the Frame Buffer
	is_frame_buffer_ready = 1;
}

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

void frame_buffer_draw(uint64_t x, uint64_t y, uint32_t color) {
	uint64_t pixel_offset = ( x * (frame_buffer_depth >> 3) ) + ( y * frame_buffer_pitch );

	frame_buffer_virtual_base_address[pixel_offset+2] = color >> 16;			// R
	frame_buffer_virtual_base_address[pixel_offset+1] = color >> 8 & 0xFF;		// G
	frame_buffer_virtual_base_address[pixel_offset] = color & 0xFF;				// B
	frame_buffer_virtual_base_address[pixel_offset+3] = 0xFF;					// A
}