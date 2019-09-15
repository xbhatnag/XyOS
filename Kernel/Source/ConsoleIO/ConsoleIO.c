#include "ConsoleIO.h"
#include "UART.h"
#include "FrameBuffer.h"
#include "FrameBufferGraphics.h"
#include <stdint.h>

/*
 * These are the methods that control I/O to FrameBuffer or UART
 */
void putc(char c){
    if (is_frame_buffer_ready) {
        frame_buffer_putc(c, 0xFFFFFF);
    } else {
        uart_output(c);
    }
}

char getc(){
    return uart_input();
}

/*
 * These methods are for general Console IO
 */
void print(char* s){
    while(*s != '\0') {
    	putc(*s);
        s++;
    }
}

void println(char* s){
    print(s);
    newline();
}

void newline(){
    putc('\n');
    putc('\r');
}

void puth_64(uint64_t num) {
    putc('0');
    putc('x');
    uint32_t should_print = 0;
    for(uint32_t i = 16; i >= 1; i--) {
        uint32_t hex = (num >> ((i-1) * 4)) & 0xF;
        should_print |= (hex || i == 1);
        if (should_print) {
            if (hex >= 10) {
                putc((hex-10) + 'a');
            } else {
                putc(hex + '0');
            }
        }
    }
}

void puth_32(uint32_t num) {
    putc('0');
    putc('x');
    uint32_t should_print = 0;
    for(uint32_t i = 8; i >= 1; i--) {
        uint32_t hex = (num >> ((i-1) * 4)) & 0xF;
        should_print |= (hex || i == 1);
        if (should_print) {
            if (hex >= 10) {
                putc((hex-10) + 'a');
            } else {
                putc(hex + '0');
            }
        }
    }
}


void puti_64(uint64_t num) {
	// At most 20 digits
	char c[20];
	int i = 0;

	do {
		c[i] = (num % 10) + '0';
		i+=1;
		num = num / 10;
	} while (num != 0 || i == 20);

	for (int j = i - 1; j >= 0; j--) {
		putc(c[j]);
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
	} while (num != 0 || i == 10);

	for (int j = i - 1; j >= 0; j--) {
		putc(c[j]);
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
    putb_64(val, 63, 60);
    putc(' ');
    putb_64(val, 59, 56);
    putc(' ');
    putb_64(val, 55, 52);
    putc(' ');
    putb_64(val, 51, 48);
    putc(' ');
    putb_64(val, 47, 44);
    putc(' ');
    putb_64(val, 43, 40);
    putc(' ');
    putb_64(val, 39, 36);
    putc(' ');
    putb_64(val, 35, 32);
    putc(' ');
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

void puth_with_title_32(char* title, uint32_t val) {
	print(title);
	puth_32(val);
	newline();
}

void puth_with_title_64(char* title, uint64_t val) {
	print(title);
	puth_64(val);
	newline();
}
