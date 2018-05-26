#include "uart.h"
#include "gpio.h"
#include "io.h"

void print(char* source) {
  char* cur = source;
  while(*cur) {
  	UART_output(*cur++);
  }
}

void clrscr() {
  print("\033c"); // ANSI escape char for Ctrl + L
}

void wait_on_any_input() {
  while(!(get(AUX_MU_STAT_REG)&1)) {}
}

char get_char_from_input() {
  return get(AUX_MU_IO_REG);
}

void typewriter() {
  // Pipe serial input -> serial output
  while(1) {
  	wait_on_any_input();
	char input = get_char_from_input();
	if (input == '\r') {
		// Some confusion b/w the DOS and UNIX standard
		// for newlines.
		print("\r\n");
		GPIO_set(11, GPIO_LEV_HIGH);
	} else if (input == 127) {
		print("\b \b");
		GPIO_set(10, GPIO_LEV_HIGH);
	} else {
		UART_output(input);
	}
  }
}
