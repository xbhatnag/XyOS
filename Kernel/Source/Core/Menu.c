#include "ConsoleIO.h"
#include "FrameBuffer.h"
#include "FrameBufferGraphics.h"
#include "Menu.h"
#include "Threading.h"

void force_exception(){
	println("Forcing unaligned data access...");
	*((volatile uint32_t*)0x3);
	println("Return from exception");
}

void get_edid_blocks(){
	uint8_t block_num = 0;
	while (frame_buffer_get_edid_block(block_num)) {
		newline();
		block_num += 1;
	}
}

void typewriter_mode(){
	println("Typewriter Mode On");
	while(1) {
		char input = getc();
		switch (input) {
			// Ctrl + C
			case 3: {
				newline();
				return;
			}
			default: {
				putc(input);
			}
		}
	}
}

void print_all_ascii_chars() {
	char c = 0;
	do {
		putc(c);
		c++;
	} while(c != 0);
	newline();
}

void test_stack_explode(uint64_t x, uint64_t y) {
    test_stack_explode(++x, --y);
}

void main_menu_choice(char c){
	switch(c) {
		case 'x': {
			frame_buffer_draw_square_corner(0xFFFFFF);
			break;
		}
		case 'e': {
			force_exception();
			break;
		}
		case 'c': {
			frame_buffer_clear();
			break;
		}
		case 'p': {
			print_all_ascii_chars();
			break;
		}
		case 'q': {
			test_stack_explode(0, 0);
			break;
		}
		case 't': {
			typewriter_mode();
			break;
		}
		default: {
			println("Unknown command");
		}
	}
}

_Noreturn void main_menu() {
	newline();
	println("\U0001F600 Welcome to XyOS");
	while(1) {
		// Wait for input!
		putstr("> ");

		// Get character
		char input = getc();
		putc(input);
		newline();

		// Do the action
		main_menu_choice(input);
	}
}
