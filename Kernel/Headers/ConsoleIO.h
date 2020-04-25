#ifndef _CONSOLE_IO_H
#define _CONSOLE_IO_H

#include <stdint.h>

/*
 *  HELPER FUNCTIONS
 */

// Support for variable arguments
// typedef char *va_list;
// #define __va_argsiz(t)          (((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
// #define va_start(ap, pN)        ((ap) = ((va_list) __builtin_next_arg(pN)))
// #define va_end(ap)              ((void)0)
// #define va_arg(ap, t)           (((ap) = (ap) + __va_argsiz(t)), *((t*) (void*) ((ap) - __va_argsiz(t))))

void printf(char *fmt, ...);
void println(const char *str);
void newline();
void putstr(const char *str);
void putc(char c);
char getc();

#endif
