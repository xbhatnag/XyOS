#include "gpio.h"

void GPIO_mode(int pin, int mode) {
  int current_mode = 0;
  int addr = 0;
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
  current_mode = get(addr);
  current_mode |= mode << ((pin-offset)*3);
  set(addr, current_mode);
}

void GPIO_start_set(int pin) {
  if (pin >= 0 && pin <= 31) {
  	set(GPIO_SET0, GPIO_SET << pin);
  } else {
  	set(GPIO_SET1, GPIO_SET << (pin-32));
  }
}

void GPIO_clear(int pin) {
  if (pin >= 0 && pin <= 31) {
  	set(GPIO_CLR0, GPIO_CLR << pin);
  } else {
  	set(GPIO_CLR1, GPIO_CLR << (pin-32));
  }
}

void GPIO_set(int pin, int level) {
  GPIO_start_set(pin);
  if (pin >= 0 && pin <= 31) {
  	set(GPIO_LEV0, level << pin);
  } else {
  	set(GPIO_LEV1, level << (pin-32));
  }
}
