#include "ConsoleIO.h"
#include "FrameBuffer.h"
#include "Menu.h"
#include "ThreadHandling.h"

void force_exception(){
	println("Forcing unaligned data access...");
	*((volatile uint32_t*)0x3);
	println("Return from exception");
}

void typewriter_mode() {
	println("Typewriter Mode On");
	while(1) {
		char input = getc();
		// Break on Ctrl-C
		if (input == 3) {
			newline();
			return;
		} else {
			putc(input);
		}
	}
}

void main_menu_choice(char c){
	switch(c) {
		case 'a': {
			frame_buffer_allocate();
			break;
		}
		case 'b': {
			uint32_t block_num = 0;
			while (frame_buffer_get_edid_block(block_num)) {
				block_num += 1;
				newline();
				newline();
			}
			break;
		}
		case 'c': {
			puti_64(counter);
			newline();
			break;
		}
		case 'd': {
			frame_buffer_blank_screen();
			break;
		}
		case 'e': {
			force_exception();
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
