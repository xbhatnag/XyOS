#include "UART.h"
#include <stdint.h>

// Control Characters
#define STX 'S'
#define ETX 'E'

void main() {
	// Setup UART
	uart_setup();

	// Pointer to start of bootloader memory
	// We will copy data byte by byte
	unsigned char * region = (unsigned char *) 0xC0000;

	// Wait for STX control character
	unsigned char data;
	while(1) {
		data = uart_input();
		if (data == STX) break;
	}

	// Get actual data
	while(1) {
		// We get data from UART in 8 byte chunks.
		for(uint32_t i = 0; i < 8; i++) {
			// Get a byte from UART
			data = uart_input();

			// Copy byte to memory
			*region = data;

			// Move memory pointer up by 1 byte
			region += 1;
		}

		// Is there more data?
		data = uart_input();
		if (data == ETX) break;
	}

	// Wait here while we switch back to the terminal
	while(1) {
		data = uart_input();
		if (data == ' ') break;
	}

	// Launch Bootloader!
	asm volatile("ldr x0, =0xC0000");
	asm volatile("br x0");

	// We should never reach here...
}
