#ifndef __FRAME_BUFFER_GRAPHICS_H__
#define __FRAME_BUFFER_GRAPHICS_H__

void frame_buffer_putc(uint8_t c, uint32_t color);
void frame_buffer_paint(uint32_t color);
void frame_buffer_clear();
void frame_buffer_draw_border(uint32_t color);
void frame_buffer_draw_cross(uint32_t x_offset, uint32_t y_offset, uint32_t size, uint32_t line_thickness, uint32_t color);
void frame_buffer_draw_cross_full_screen(uint32_t color);
void frame_buffer_draw_cross_corner(uint32_t color);
void frame_buffer_draw_square_corner(uint32_t color);
void frame_buffer_puti_64(uint64_t num);

#endif