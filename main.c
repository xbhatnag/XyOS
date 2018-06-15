#include "gpio.h"
#include "uart.h"
#include "stdio.h"

void main() {
	// Setup UART1
	uart_setup();

	// Print something
	println("Hello World!");
	
	// Setup GPIO Traffic Lights
	gpio_mode(9, GPIO_MODE_OUTPUT);
	gpio_mode(10, GPIO_MODE_OUTPUT);
	gpio_mode(11, GPIO_MODE_OUTPUT);

	// Turn them all on
	gpio_set(9, GPIO_LEVEL_HIGH);
	gpio_set(10, GPIO_LEVEL_HIGH);
	gpio_set(11, GPIO_LEVEL_HIGH);
	
	// We don't want to run off the end
	// of the instruction set.
	// So we have an infinite loop.
	while(1) {
		// Typewriter mode!
		putc(getc());
	}
}
