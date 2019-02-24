#include "Peripherals.h"
#include "GPIO.h"
#include "UART.h"

#define AUX_BASE 	(MMAP_IO_BASE + 0x215000)
#define AUX_STATE	((volatile unsigned*)(AUX_BASE + 0x04))
#define AUX_MU_IO_REG	((volatile unsigned*)(AUX_BASE + 0x40))
#define AUX_MU_IER_REG	((volatile unsigned*)(AUX_BASE + 0x44))
#define AUX_MU_IIR_REG	((volatile unsigned*)(AUX_BASE + 0x48))
#define AUX_MU_LCR_REG	((volatile unsigned*)(AUX_BASE + 0x4C))
#define AUX_MU_MCR_REG	((volatile unsigned*)(AUX_BASE + 0x50))
#define AUX_MU_LSR_REG	((volatile unsigned*)(AUX_BASE + 0x54))
#define AUX_MU_CNTL_REG	((volatile unsigned*)(AUX_BASE + 0x60))
#define AUX_MU_STAT_REG	((volatile unsigned*)(AUX_BASE + 0x64))
#define AUX_MU_BAUD	((volatile unsigned*)(AUX_BASE + 0x68))

void uart_setup() {
  register unsigned int r;
  
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

void uart_output(unsigned int letter) {
  do {asm volatile("nop");} while (!(*AUX_MU_LSR_REG & 0x20));
  *AUX_MU_IO_REG = letter;
}

unsigned int uart_input() {
  // Busy wait until FIFO holds at least 1 symbol
  do {asm volatile("nop");} while (!(*AUX_MU_LSR_REG & 0x1));
  return *AUX_MU_IO_REG;
}


