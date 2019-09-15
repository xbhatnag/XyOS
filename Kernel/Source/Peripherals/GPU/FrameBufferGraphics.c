#include "FrameBuffer.h"
#include "FrameBufferGraphics.h"
#include "ConsoleFont.h"
#include <stdint.h>

#define FRAME_BUFFER_BACKGROUND 0x26ca

uint32_t current_x = 0;
uint32_t current_y = 0;

void frame_buffer_draw_cross(uint32_t color) {
	uint64_t box_size = frame_buffer_height;
	uint64_t line_length = 50;
	uint64_t y = 0;
	uint64_t left_x = (frame_buffer_width - frame_buffer_height) / 2;
	uint64_t right_x = left_x + box_size;
	for (uint64_t y_offset = 0; y_offset < box_size; y_offset++) {
		for (uint64_t x_offset = 0; x_offset < line_length; x_offset++) {
			frame_buffer_draw(left_x + x_offset, y + y_offset, color);
			frame_buffer_draw(right_x - x_offset, y + y_offset, color);
		}
		left_x += 1;
		right_x -= 1;
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

void frame_buffer_draw_char(uint64_t x, uint64_t y, uint8_t c, uint32_t color) {
	for (uint16_t y_offset = 0; y_offset < 16; y_offset++) {
		for (uint64_t x_offset = 0; x_offset < 8; x_offset++) {
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

void frame_buffer_putc(char c, uint32_t color){
	switch (c) {
		case '\n': {
			current_y += 16;
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