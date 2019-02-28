#include "GPIO.h"

void gpio_mode(int pin, int mode) {
  volatile uint32_t* addr;
  int current_mode = 0;
  int offset = 0;
  if (pin >= 0 && pin <= 9) {
    addr = GPIO_SEL0;
    offset = 0;
  } else if (pin >= 10 && pin <= 19) {
  	addr = GPIO_SEL1;
	  offset = 10;
  } else if (pin >= 20 && pin <= 29) {
  	addr = GPIO_SEL2;
	  offset = 20;
  } else if (pin >= 30 && pin <= 39) {
  	addr = GPIO_SEL3;
	  offset = 30;
  } else if (pin >= 40 && pin <= 49) {
  	addr = GPIO_SEL4;
	  offset = 40;
  } else {
  	addr = GPIO_SEL5;
	  offset = 50;
  }
  current_mode = *addr;
  current_mode |= mode << ((pin-offset)*3);
  *addr = current_mode;
}

void gpio_start_set(int pin) {
  if (pin >= 0 && pin <= 31) {
  	*GPIO_SET0 = 1 << pin;
  } else {
  	*GPIO_SET1 = 1 << (pin-32);
  }
}

void gpio_clear(int pin) {
  if (pin >= 0 && pin <= 31) {
  	*GPIO_CLR0 = 1 << pin;
  } else {
  	*GPIO_CLR1 = 1 << (pin-32);
  }
}

void gpio_set(int pin, int level) {
  gpio_start_set(pin);
  if (pin >= 0 && pin <= 31) {
  	*GPIO_LEV0 = level << pin;
  } else {
  	*GPIO_LEV1 = level << (pin-32);
  }
}
