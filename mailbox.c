#include "mailbox.h"
#include "stdio.h"
#include "error.h"

#define MAILBOX_NUM_CLOCKS 10

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

void mailbox_print_clock_current_frequency(uint32_t id) {
	uint32_t __attribute__((aligned(16))) message[8];
	// message header
	message[0] = sizeof(message);	// Message Size
	message[1] = 0;			// Request Code

	// tag 1 header
	message[2] = 0x00030005;	// Get clock state
	message[3] = 8;			// 8 bytes in response
	message[4] = 0;			// Request Code

	// tag 1 data
	message[5] = id;		// Clock ID
	message[6] = 0;			// Clock frequency

	// end tag
	message[7] = 0;

	// write to mailbox	
	mailbox_write(&message[0]);

	// wait until read is possible
	mailbox_wait_read();
	
	if (message[1] != 0x80000000) {
		error("Could not get clock current frequency. Mailbox write failed.");
	}

	// Print clock frequency
	print("(");
	puti_32(message[6]/1000);
	print(" KHz) ");
}

void mailbox_print_clock_state(uint32_t id) {
	uint32_t __attribute__((aligned(16))) message[8];
	// message header
	message[0] = sizeof(message);	// Message Size
	message[1] = 0;			// Request Code

	// tag 1 header
	message[2] = 0x00030001;	// Get clock state
	message[3] = 8;			// 8 bytes in response
	message[4] = 0;			// Request Code

	// tag 1 data
	message[5] = id;		// Clock ID
	message[6] = 0;			// Clock state

	// end tag
	message[7] = 0;

	// write to mailbox	
	mailbox_write(&message[0]);

	// wait until read is possible
	mailbox_wait_read();
	
	if (message[1] != 0x80000000) {
		error("Could not get clock state. Mailbox write failed.");
	}

	// Print clock state
	uint32_t enabled = message[6] & 0x1;
	uint32_t does_not_exist = message[6] & 0x2;
	
	if (does_not_exist) {
		error("Retrieving clock state for clock that does not exist!");
	}

	if (enabled) {
		print("(On) ");
	} else {
		print("(Off) ");
	}
}

void mailbox_print_clock_name(uint32_t id) {
	switch(id) {
		case 1: print("(EMMC)"); break;
		case 2: print("(UART)"); break;
		case 3: print("(ARM)"); break;
		case 4: print("(CORE)"); break;
		case 5: print("(V3D)"); break;
		case 6: print("(H264)"); break;
		case 7: print("(ISP)"); break;
		case 8: print("(SDRAM)"); break;
		case 9: print("(PIXEL)"); break;
		case 10: print("(PWM)"); break;
		default: error("Unknown clock. Cannot retrieve name.");
	}
	print(" ");
}

void mailbox_print_all_clocks() {
	uint32_t __attribute__((aligned(16))) message[6 + (MAILBOX_NUM_CLOCKS * 2)];
	// message header
	message[0] = sizeof(message);	// Message Size
	message[1] = 0;			// Request Code

	// tag 1 header
	message[2] = 0x00010007;		// Get clocks
	message[3] = MAILBOX_NUM_CLOCKS * 8;	// Bytes in response (8 bytes per clock)
	message[4] = 0;				// Request Code

	// end tag
	message[6 + (MAILBOX_NUM_CLOCKS * 2) - 1] = 0;

	// write to mailbox	
	mailbox_write(&message[0]);

	// wait until read is possible
	mailbox_wait_read();

	if (message[1] != 0x80000000) {
		error("Could not get clocks. Mailbox write failed.");
	}

	// Print all clock data
	for (int i = 0; i < MAILBOX_NUM_CLOCKS; i++) {
		uint32_t parent_id = message[5+(i*2)];
		uint32_t id = message[5+(i*2)+1];
		print("Clock ");
		puti_32(id);
		print(" -> ");

		mailbox_print_clock_name(id);
		
		if (parent_id == 0) {
			print("(Base) ");
		} else {
			print("(Parent ");
			puti_32(parent_id);
			print(") ");
		}

		mailbox_print_clock_state(id);
		mailbox_print_clock_current_frequency(id);

		newline();
	}
}

uint32_t mailbox_get_system_temp() {
	uint32_t __attribute__((aligned(16))) message[8];
	// message header
	message[0] = sizeof(message);	// Message Size
	message[1] = 0;			// Request Code

	// tag 1 header
	message[2] = 0x00030006;	// Get temperature
	message[3] = 8;			// 8 bytes in response
	message[4] = 0;			// Request Code

	// tag 1 data
	message[5] = 0;			// Temperature ID (should be 0)
	message[6] = 0;			// Temperature (in 1000ths of a °C)

	// end tag
	message[7] = 0;

	// write to mailbox	
	mailbox_write(&message[0]);

	// wait until read is possible
	mailbox_wait_read();
	
	if (message[1] != 0x80000000) {
		error("Could not get system temperature. Mailbox write failed.");
	}

	return message[6]/1000;
}
