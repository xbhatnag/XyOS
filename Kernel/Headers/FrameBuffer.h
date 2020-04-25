#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__

#include <stdint.h>

#define FRAME_BUFFER_PADDR_MASK 0x3fffffff;

unsigned char* frame_buffer_virtual_base_address;
volatile uint32_t frame_buffer_physical_base_address;
volatile uint32_t frame_buffer_height;
volatile uint32_t frame_buffer_width;
uint32_t frame_buffer_size;
uint32_t frame_buffer_pitch;
uint32_t frame_buffer_depth;

uint32_t frame_buffer_get_edid_block(uint32_t index);
void frame_buffer_init(uint32_t width, uint32_t height);
volatile void frame_buffer_draw(uint32_t x, uint32_t y, uint32_t color);

#endif
