#include "gpio.h"
#include "uart.h"
#include "io.h"

__inline void IRQ_enable_global() {
  asm volatile("CPSIE I");
}

void TrafficLight_setup() {
  // Set pin 9, 10, and 11 to output mode
  GPIO_mode(9,GPIO_MODE_OUTPUT);
  GPIO_mode(10, GPIO_MODE_OUTPUT);
  GPIO_mode(11, GPIO_MODE_OUTPUT);
}

int main() {
  IRQ_enable_global();
  
  // Setup Traffic Light
  TrafficLight_setup();

  // Turn Red ON
  GPIO_set(9,GPIO_LEV_HIGH);

  // Setup UART
  UART_setup();
  
  // Clear the screen
  clrscr();

  // Start sending data
  print("-> Hello World!\r\nWaiting on input : ");

  // Behave like a typewriter.
  typewriter();

  // Infinite loop on the CPU.
  // Otherwise we reach the end of the instruction set.
  while(1) {
  }
  return 0;
}
