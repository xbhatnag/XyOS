#ifndef _UART_H_
#define _UART_H_

void uart_setup();
void uart_flush();
void uart_output(unsigned int letter);
unsigned int uart_input();

#endif
