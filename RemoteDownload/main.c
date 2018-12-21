#include "uart.h"

// Control Characters
#define STX 83
#define ETX 69
#define ACK 65
#define NAK 78

void main() {
	// Setup UART
	uart_setup();
	
	// Pointer to start of kernel memory
	// We will copy data byte by byte
	unsigned char * region = (unsigned char *) 0x80000;

	while(1) {
		// Get control character from UART
		unsigned control = uart_input();

		// Control character is either STX or ETX
		if (control != STX && control != ETX) {
			// Unrecognized character
			uart_output(NAK);
			continue;
		}

		// Acknowledge control character
		uart_output(ACK);
		
		// If ETX, then all data received!
		if (control == ETX) break;

		// Get data from UART
		unsigned char data = uart_input();
	    
		// Copy data to memory
		*region = data;

		// Move memory pointer up by 1 byte
	    region += 1;

		// Acknowledge data
		uart_output(ACK);
	}

	while(1) {
		// Get character from UART
		unsigned i = uart_input();

		if (i != ' ') continue;

		// Boot OS!
		asm volatile("ldr x0, =0x80000");
		asm volatile("br x0");

		// We should never reach here.
	}
}
