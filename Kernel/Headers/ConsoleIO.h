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

void print(char* s);
void println(char* s);
void newline();

char getc();

#endif
