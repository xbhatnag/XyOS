#include "gpio.h"
#include "uart.h"
#include "stdio.h"
#include "timer.h"
#include "interrupt.h"

extern char implementer();
extern int variant();
extern int exception_level();
void menu(char c);

char* implementer_str() {
	if (implementer() == 'A') {
		return "ARM";
	}
	return "Unknown";
}

char* variant_str() {
	if (variant() == 0) {
		return "Cortex A53";
	}
	return "Unkown";
}

char exception_level_char(){
	int e_level = exception_level();
	switch(e_level) {
		case 0: return '0';
		case 1: return '1';
		case 2: return '2';
		case 3: return '3';
		default: return 'U'; // Unknown
	}
}

void main() {
	// Setup UART1
	uart_setup();

	// Setup GPIO Traffic Lights
	gpio_mode(9, GPIO_MODE_OUTPUT);
	gpio_mode(10, GPIO_MODE_OUTPUT);
	gpio_mode(11, GPIO_MODE_OUTPUT);

	println("-> CPU Information");
	printf("Implementer : ");
	println(implementer_str());
	
	printf("Variant : ");
	println(variant_str());
	
	printf("Current Exception Level : ");
	putc(exception_level_char());
	newline();

	//println("-> Timer");
	//timer_enable();

	println("-> Interrupts");
	enable_interrupts();

	// We don't want to run off the end
	// of the instruction set.
	// So we have an infinite loop.
	while(1) {
		// Wait for input!
		printf("> ");
		char input = getc();
		putc(input);
		newline();
		menu(input);
	}
}

void force_exception(){
	println("Forcing unaligned data access...");
	*((volatile unsigned*)0x3);
}

void menu(char c){
	switch(c) {
		case 'e': {
			force_exception();
			break;
		}
		case 'r': {
			println("Stop says the red light");
			gpio_set(9, GPIO_LEVEL_HIGH);
			break;
		}
		case 'g': {
			println("Go says the green");
			gpio_set(11, GPIO_LEVEL_HIGH);
			break;
		}
		case 'y': {
			println("Wait says the yellow one");
			println("Blinking in between...");
			gpio_set(10, GPIO_LEVEL_HIGH);
			break;
		}
		default: {
			println("Unknown command");
		}
	}
}
