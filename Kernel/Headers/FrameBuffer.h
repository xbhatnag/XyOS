#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

#include <stdint.h>

void frame_buffer_allocate();
uint32_t frame_buffer_get_edid_block(uint32_t);
void frame_buffer_blank_screen();

#endif
