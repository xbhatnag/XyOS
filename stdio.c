#include "stdio.h"
#include "uart.h"
#include <stdint.h>

void print(char* s){
    while(*s != '\0') {
    	uart_output(*s);
	s++;
    }
}

void println(char* s){
    print(s);
    newline();
}

void putc(char c){
    uart_output(c);
}

char getc(){
   char c = uart_input();
   return c;
}

void newline(){
   uart_output('\n');
   uart_output('\r');
}

void puti_64(uint64_t num) {
	// At most 10 digits
	char c[10];
	int i = 0;

	do {
		c[i] = (num % 10) + '0';
		i+=1;
		num = num / 10;
	} while (num != 0);

	for (int j = i - 1; j >= 0; j--) {
		uart_output(c[j]);
	}
}

void puti_32(uint32_t num) {
	// At most 10 digits
	char c[10];
	int i = 0;

	do {
		c[i] = (num % 10) + '0';
		i+=1;
		num = num / 10;
	} while (num != 0);

	for (int j = i - 1; j >= 0; j--) {
		uart_output(c[j]);
	}
}

void putb_64(uint64_t num, int end, int start) {
	for (int i = end; i >= start; i--) {
		int bit = ((num >> i) & 0x1);
		if (bit) {
			putc('1');
		} else {
			putc('0');
		}
	}
}

void putb_32(uint32_t num, int end, int start) {
	for (int i = end; i >= start; i--) {
		int bit = ((num >> i) & 0x1);
		if (bit) {
			putc('1');
		} else {
			putc('0');
		}
	}
}

void pretty_putb_64(uint64_t val) { 
	putb_64(val, 31, 28);
	putc(' ');
	putb_64(val, 27, 24);
	putc(' ');
	putb_64(val, 23, 20);
	putc(' ');
	putb_64(val, 19, 16);
	putc(' ');
	putb_64(val, 15, 12);
	putc(' ');
	putb_64(val, 11, 8);
	putc(' ');
	putb_64(val, 7, 4);
	putc(' ');
	putb_64(val, 3, 0);
}

void pretty_putb_32(uint32_t val) { 
	putb_64(val, 31, 28);
	putc(' ');
	putb_64(val, 27, 24);
	putc(' ');
	putb_64(val, 23, 20);
	putc(' ');
	putb_64(val, 19, 16);
	putc(' ');
	putb_64(val, 15, 12);
	putc(' ');
	putb_64(val, 11, 8);
	putc(' ');
	putb_64(val, 7, 4);
	putc(' ');
	putb_64(val, 3, 0);
}
