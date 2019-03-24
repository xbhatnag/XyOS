#include "ConsoleIO.h"
#include "VirtualMemory.h"
#include "KernelLoader.h"

extern void launch_kernel();
extern void set_vector_table();

void main() {
	println("## Welcome to XyOS Bootloader");
	println("┗━ Configuring Vector Table");
	set_vector_table();

	newline();

	println("## MENU");
	println("1. Download Kernel");
	println("2. Initialize Virtual Memory");
	println("3. Launch Kernel");

	while(1) {
		print("> ");
		char input = getc();
		putc(input);
		newline();
		switch (input) {
			case '1':
				download_kernel();
				break;
			case '2':
				vmem_init();
				break;
			case '3':
				launch_kernel();
				break;
			default:
				println("Unknown Option");
		}
	}
}
