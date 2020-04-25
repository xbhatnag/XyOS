#include "FrameBuffer.h"
#include "FrameBufferGraphics.h"
#include "ConsoleFont.h"
#include <stdint.h>

#define FRAME_BUFFER_BACKGROUND 0x26ca

uint32_t current_x = 0;
uint32_t current_y = 0;

// This formula is flawed.
void frame_buffer_draw_cross(uint32_t x_offset, uint32_t y_offset, uint32_t size, uint32_t line_thickness, uint32_t color) {
	for (uint32_t y = 0; y < size; y++) {
		for (uint32_t x = 0; x < line_thickness; x++) {
			frame_buffer_draw(x_offset + x + y, y_offset + y, color);
			frame_buffer_draw(x_offset + size - x - y, y_offset + y, color);
		}
	}
}

void frame_buffer_draw_cross_full_screen(uint32_t color) {
	uint64_t x_offset = (frame_buffer_width - frame_buffer_height) / 2;
	frame_buffer_draw_cross(
		x_offset,
		0,
		frame_buffer_height,
		20,
		color
	);
}

void frame_buffer_draw_cross_corner(uint32_t color) {
	frame_buffer_draw_cross(
		0,
		0,
		100,
		20,
		color
	);
}

void frame_buffer_draw_square_corner(uint32_t color) {
	for (uint64_t y = 0; y < 100; y++) {
		for (uint64_t x = 0; x < 100; x++) {
			frame_buffer_draw(frame_buffer_width - 100 + x, frame_buffer_height - 100 + y, color);
		}
	}
}

void frame_buffer_paint(uint32_t color) {
	for (uint64_t y = 0; y < frame_buffer_height; y++) {
		for (uint64_t x = 0; x < frame_buffer_width; x++) {
			frame_buffer_draw(x, y, color);
		}
	}
}

void frame_buffer_clear() {
	frame_buffer_paint(FRAME_BUFFER_BACKGROUND);
	current_x = 0;
	current_y = 0;
}

void frame_buffer_draw_char(uint32_t x, uint32_t y, uint8_t c, uint32_t color) {
	for (uint32_t y_offset = 0; y_offset < 16; y_offset++) {
		for (uint32_t x_offset = 0; x_offset < 8; x_offset++) {
			if ((FONT[c][y_offset] >> (7 - x_offset)) & 0x1) {
				frame_buffer_draw(x + x_offset, y + y_offset, color);
			} else {
				frame_buffer_draw(x + x_offset, y + y_offset, FRAME_BUFFER_BACKGROUND);
			}
		}
	}
}

void frame_buffer_draw_border(uint32_t color) {
	uint32_t line_width = 1;
	for (uint64_t x = 0; x < frame_buffer_width; x++) {
		for (uint64_t y = 0; y < line_width; y++) {
			frame_buffer_draw(x, y, color);
			frame_buffer_draw(x, frame_buffer_height - y - 1, color);	
		}
	}
	for (uint64_t y = 0; y < frame_buffer_height; y++) {
		for (uint64_t x = 0; x < line_width; x++) {
			frame_buffer_draw(x, y, color);
			frame_buffer_draw(frame_buffer_width - x - 1, y, color);
		}
	}
}

// Prints a number to the bottom right of the screen
void frame_buffer_puti_64(uint64_t num) {
	// At most 20 digits
	unsigned char c[20];
	uint32_t i = 0;

	do {
		c[i] = (num % 10) + '0';
		i+=1;
		num = num / 10;
	} while (num != 0 || i == 20);

	for (uint32_t j = 0; j < i; j++) {
		frame_buffer_draw_char((i - j - 1) * 8, 0, c[j], 0xFF0000);
	}
}

void frame_buffer_putc(uint8_t c, uint32_t color){
	switch (c) {
		case '\n': {
			current_y += 16;
			break;
		}
		case '\t': {
			frame_buffer_putc(' ', 0xFFFFFF);
			frame_buffer_putc(' ', 0xFFFFFF);
			frame_buffer_putc(' ', 0xFFFFFF);
			frame_buffer_putc(' ', 0xFFFFFF);
			break;
		}
		case '\r': {
			current_x = 0;
			break;
		}
		default: {
			frame_buffer_draw_char(current_x, current_y, c, color);
			current_x += 8;
		}
	}
	// Overflow the screen horizontally
	// Move to the next line
	if (current_x >= frame_buffer_width) {
		current_x = 0;
		current_y += 16;
	}
	// Overflow the screen vertically
	// Clear the screen
	if (current_y >= frame_buffer_height) {
		frame_buffer_clear();
	}
}