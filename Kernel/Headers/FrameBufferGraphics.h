#ifndef __FRAME_BUFFER_GRAPHICS_H__
#define __FRAME_BUFFER_GRAPHICS_H__

void frame_buffer_putc(char, uint32_t color);
void frame_buffer_paint(uint32_t color);
void frame_buffer_clear();
void frame_buffer_draw_border(uint32_t color);
void frame_buffer_draw_cross(uint32_t color);

#endif