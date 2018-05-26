#include "mmap_io.h"
#include "gpio.h"
#include "uart.h"

void UART_setup() {
  // Enable TXD1 on pin 14 of GPIO
  GPIO_mode(14, GPIO_MODE_ALT5);

  // Enable RXD1 on pin 15 of GPIO
  GPIO_mode(15, GPIO_MODE_ALT5);

  // Enable UART
  set(AUX_STATE,1);

  // Disable interrupts for RX and TX.
  // Disable access to the MS 8 bits of baud rate.
  set(AUX_MU_IER_REG, 0);

  // Disable the UART transmitter + receiver
  // This is probably to prevent data flow while changing settings.
  set(AUX_MU_CNTL_REG, 0);

  // Enable 8-bit mode on UART
  set(AUX_MU_LCR_REG, 0x3);

  // Set RTS line to low
  set(AUX_MU_MCR_REG, 0x0);

  // Clear the FIFO
  set(AUX_MU_IIR_REG, 0x7);

  // UART is oversampled 8x
  // Baudrate = (System Clock Speed/(8*(Baud + 1))
  // We want Baudrate = 115200
  // We know System Clock Speed = 250,000,000
  // From the formula, we get Baud = 270
  set(AUX_MU_BAUD, 270);

  // Enable the UART transmitter + receiver. We should be good to go at this point.
  set(AUX_MU_CNTL_REG,3);

  // Enable interrupts for RX
  // Keep interrupts disabled for TX and disable access to the MS 8 bits of baud rate.
  //set(AUX_MU_IER_REG, 1)
}

void UART_flush() {
  // Busy wait until FIFO is empty
  while(1) {
	if (get(AUX_MU_LSR_REG)&0x20) break;
  }
}

void UART_output(char letter) {
  UART_flush();
  set(AUX_MU_IO_REG, letter);
}


