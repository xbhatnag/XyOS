#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdint.h>

void putc(char c);
void puti_64(uint64_t num);
void puti_32(uint32_t num);
void puth_64(uint64_t num);
void puth_32(uint32_t num);
void putb_64(uint64_t num, int end, int start);
void putb_32(uint32_t num, int end, int start);
void pretty_putb_64(uint64_t num);
void pretty_putb_32(uint32_t num);
void puth_with_title_32(char* title, uint32_t val);
void puth_with_title_64(char* title, uint64_t val);
void puti_with_title_32(char* title, uint32_t val);
void puti_with_title_64(char* title, uint64_t val);

void print(char* s);
void println(char* s);
void newline();

char getc();

#endif
