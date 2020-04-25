#include "ConsoleIO.h"
#include "VirtualMemory.h"
#include "KernelLoader.h"
#include "UART.h"

extern void launch_kernel();
extern void set_vector_table();

void main() {
	uart_setup();
	println("## Welcome to XyOS Bootloader");
	println("Configuring Vector Table...");
	set_vector_table();
	println("Waiting for Kernel over UART...");
	download_kernel();
	getc();
	vmem_init();
	println("Press any key to boot into kernel...");
	getc();
	println("------------------------------------");
	launch_kernel();
}
