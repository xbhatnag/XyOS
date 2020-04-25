#include "ConsoleIO.h"
#include "UART.h"
#include "FrameBuffer.h"
#include "FrameBufferGraphics.h"
#include "Exceptions.h"
#include <stdint.h>
#include <stdarg.h>

/*
 * These are the methods that control I/O to FrameBuffer or UART
 */
void putc(char c){
    if (c == '\n') {
        uart_output('\r');
    }
    uart_output(c);
}

void puth_64(uint64_t num) {
    putc('0');
    putc('x');
    uint32_t should_print = 0;
    for(uint32_t i = 16; i >= 1; i--) {
        uint32_t hex = (num >> ((i-1) * 4)) & 0xF;
        should_print |= (hex || i == 1);
        if (should_print) {
            if (hex >= 10) {
                putc((hex-10) + 'a');
            } else {
                putc(hex + '0');
            }
        }
    }
}

char getc(){
    return uart_input();
}

char c2x( char ch ) {
    if ( (ch <= 9) ) return '0' + ch;
    return 'a' + ch - 10;
}

int a2d(char ch) {
    if( ch >= '0' && ch <= '9' ) return ch - '0';
    if( ch >= 'a' && ch <= 'f' ) return ch - 'a' + 10;
    if( ch >= 'A' && ch <= 'F' ) return ch - 'A' + 10;
    return -1;
}

char a2i(char ch, const char **src, int base, int *nump) {
    int num, digit;
    const char *p;

    p = *src; num = 0;
    while(( digit = a2d(ch) ) >= 0 ) {
        if ( digit > base ) break;
        num = num*base + digit;
        ch = *p++;
    }
    *src = p; *nump = num;
    return ch;
}

void ui2a(uint64_t num, unsigned int base, char *bf) {
    int n = 0;
    int dgt;
    uint64_t d = 1;

    while( (num / d) >= base ) d *= base;

    while( d != 0 ) {
        dgt = num / d;
        num %= d;
        d /= base;
        if( n || dgt > 0 || d == 0 ) {
            *bf++ = dgt + ( dgt < 10 ? '0' : 'a' - 10 );
            ++n;
        }
    }
    *bf = 0;
}

void i2a(int64_t num, char *bf) {
    if( num < 0 ) {
        num = -num;
        *bf++ = '-';
    }
    ui2a(num, 10, bf);
}

/*
 * I/O FUNCTIONS
 */
/*
 * This function does not protect you against race conditions
 */
void putstr(const char *str) {
    while( *str ) {
        putc( *str );
        str++;
    }
}

void newline() {
    putc('\n');
    putc('\r');
}

void println(const char *str) {
    while( *str ) {
        putc( *str );
        str++;
    }
    newline(putc);
}

void putw(int n, char fc, char *bf) {
    char ch;
    char *p = bf;

    while( *p++ && n > 0 ) n--;
    while( n-- > 0 ) putc( fc );
    while( ( ch = *bf++ ) ) putc( ch );
}

void format(const char *fmt, va_list va) {
    char bf[21];
    char ch, lz;
    int w;

    while ( ( ch = *(fmt++) ) ) {
        if ( ch != '%' )
            putc( ch );
        else {
            lz = 0; w = 0;
            ch = *(fmt++);
            switch ( ch ) {
                case '0':
                    lz = 1; ch = *(fmt++);
                    break;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    ch = a2i(ch, &fmt, 10, &w);
                    break;
            }
            switch( ch ) {
                case 0: return;
                case 'c':
                    putc( va_arg( va, int ) );
                    break;
                case 's':
                    putw(w, 0, va_arg(va, char*));
                    break;
                case 'u':
                    ui2a(va_arg(va, uint64_t), 10, bf);
                    putw(w, lz, bf);
                    break;
                case 'd':
                    i2a(va_arg(va, int64_t), bf);
                    putw(w, lz, bf);
                    break;
                case 'x':
                    ui2a(va_arg(va, uint64_t), 16, bf);
                    putc('0');
                    putc('x');
                    putw(w, lz, bf);
                    break;
                case '%':
                    putc(ch);
                    break;
            }
        }
    }
}

void printf(char *fmt, ...) {
    va_list va;
    va_start(va,fmt);

    format(fmt, va);
    va_end(va);
}
