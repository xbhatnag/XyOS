#include "GPIO.h"
#include "UART.h"

volatile void uart_setup() {
  register uint32_t r;

  // Enable UART
  *AUX_STATE |= 1;

  // Disable interrupts for RX and TX.
  // Disable access to the MS 8 bits of baud rate.
  *AUX_MU_IER_REG = 0;

  // Disable the UART transmitter + receiver
  // This is probably to prevent data flow while changing settings.
  *AUX_MU_CNTL_REG = 0;

  // Enable 8-bit mode on UART
  *AUX_MU_LCR_REG = 0x3;

  // Set RTS line to low
  *AUX_MU_MCR_REG = 0x0;

  // Clear the FIFO
  *AUX_MU_IIR_REG = 0xc6;

  // UART is oversampled 8x
  // Baudrate = (System Clock Speed/(8*(Baud + 1))
  // We want Baudrate = 115200
  // We know System Clock Speed = 250,000,000
  // From the formula, we get Baud = 270
  *AUX_MU_BAUD = 270;

  // Enable TXD1 on pin 14 of GPIO
  gpio_mode(14, GPIO_MODE_ALT5);

  // Enable RXD1 on pin 15 of GPIO
  gpio_mode(15, GPIO_MODE_ALT5);

  // Control the actuation of both pins
  *GPIO_PUD = 0;

  // Run some clock cycles
  r=150; while(r--) { asm volatile("nop"); }

  *GPIO_PUD_CLK0 = (1<<14)|(1<<15);

  // Run some clock cycles
  r=150; while(r--) { asm volatile("nop"); }

  *GPIO_PUD_CLK0 = 0;

  // Enable the UART transmitter + receiver. We should be good to go at this point.
  *AUX_MU_CNTL_REG = 3;
}

volatile void uart_output_flush() {
  // Busy wait until transmitter is idle
  do {asm volatile("nop");} while (!(*AUX_MU_LSR_REG & 0x20));
}

volatile void uart_output(uint32_t letter) {
  uart_output_flush();
  *AUX_MU_IO_REG = letter;
}

volatile void uart_input_flush() {
  // Busy wait until FIFO holds at least 1 symbol
  do {asm volatile("nop");} while (!(*AUX_MU_LSR_REG & 0x1));
}

volatile uint32_t uart_input() {
  uart_input_flush();
  return *AUX_MU_IO_REG;
}
