#include "ConsoleIO.h"
#include "KernelLoader.h"
#include "UART.h"

// Control Characters
#define STX 'S'
#define ETX 'E'

uint64_t kernel_size = 0;

void download_kernel() {
	// Pointer to start of kernel memory
	// We will copy data byte by byte
	unsigned char * region = (unsigned char *) KERNEL_PHYSICAL_START;

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

			// Count 1 more byte
			kernel_size += 1;
		}

		// Is there more data?
		data = uart_input();
		if (data == ETX) break;
	}

	// Wait here while we switch back to the terminal
	while(1) {
		// Get character from UART
		data = uart_input();
		if (data == ' ') break;
		uart_output(data);
	}

	println("Kernel downloaded successfully");
}
