#include "ConsoleIO.h"
#include "FrameBufferGraphics.h"
#include "Menu.h"
#include "ThreadHandling.h"

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

void main_menu_choice(char c){
	switch(c) {
		case 'x': {
			frame_buffer_draw_cross(0xFFFFFF);
			break;
		}
		case 'e': {
			get_edid_blocks();
			break;
		}
		case 'c': {
			frame_buffer_clear();
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

void main_menu() {
	println("MAIN MENU");
	while(1) {
		// Wait for input!
		print("> ");

		// Get character
		char input = getc();
		putc(input);
		newline();

		// Do the action
		main_menu_choice(input);
	}
}
