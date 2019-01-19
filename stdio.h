#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdint.h>

void putc(char c);
void puti(uint32_t num);
void putb(uint64_t num, int end, int start);
void pretty_putb_32(uint64_t num);


void print(char* s);
void println(char* s);
void newline();

char getc();

#endif
