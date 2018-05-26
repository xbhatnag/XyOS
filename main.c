#include <stdint.h>

enum {
  IO_BASE = 0x3F000000,
  IO_END = 0x3FFFFFFF
};

enum {
  GPIO_BASE = IO_BASE + 0x200000,
  GPIO_SEL0 = GPIO_BASE,
  GPIO_SEL1 = GPIO_BASE + 0x4,
  GPIO_SEL2 = GPIO_BASE + 0x8,
  GPIO_SEL3 = GPIO_BASE + 0xC,
  GPIO_SEL4 = GPIO_BASE + 0x10,
  GPIO_SEL5 = GPIO_BASE + 0x14,
  GPIO_SET0 = GPIO_BASE + 0x1C,
  GPIO_SET1 = GPIO_BASE + 0x20,
  GPIO_CLR0 = GPIO_BASE + 0x28,
  GPIO_CLR1 = GPIO_BASE + 0x2C,
  GPIO_LEV0 = GPIO_BASE + 0x34,
  GPIO_LEV1 = GPIO_BASE + 0x38
};

enum {
  GPIO_MODE_INPUT = 0x0,
  GPIO_MODE_OUTPUT = 0x1,
  GPIO_MODE_ALT5 = 0x2,
  GPIO_SET = 0x1,
  GPIO_CLR = 0x1,
  GPIO_LEV_HIGH = 0x1,
  GPIO_LEV_LOW = 0x0,
};

enum {
  AUX_BASE = IO_BASE + 0x215000,
  AUX_STATE = AUX_BASE + 0x4,
  AUX_MU_IO_REG = AUX_BASE + 0x40,
  AUX_MU_IER_REG = AUX_BASE + 0x44,
  AUX_MU_IIR_REG = AUX_BASE + 0x48,
  AUX_MU_LCR_REG = AUX_BASE + 0x4C,
  AUX_MU_MCR_REG = AUX_BASE + 0x50,
  AUX_MU_LSR_REG = AUX_BASE + 0x54,
  AUX_MU_MSR_REG = AUX_BASE + 0x58,
  AUX_MU_CNTL_REG = AUX_BASE + 0x60,
  AUX_MU_STAT_REG = AUX_BASE + 0x64,
  AUX_MU_BAUD = AUX_BASE + 0x68
};

void set(int addr,int value) {
  *(volatile uint32_t*)addr = value;
}

int get(int addr) {
  return *(volatile uint32_t*)addr;
}

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

int main() {
  // Setup - Traffic Lights
  GPIO_mode(9,GPIO_MODE_OUTPUT);
  GPIO_mode(10, GPIO_MODE_OUTPUT);
  GPIO_mode(11, GPIO_MODE_OUTPUT);
  
  // Turn Red ON
  GPIO_set(9,GPIO_LEV_HIGH);

  // Enable TX on pin 14 of GPIO
  GPIO_mode(14, GPIO_MODE_ALT5);

  // Turn Amber ON
  GPIO_set(10, GPIO_LEV_HIGH);

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

  // Different - Clear the FIFO
  set(AUX_MU_IIR_REG, 0x7);

  // UART is oversampled 8x
  // Baudrate = (System Clock Speed/(8*(Baud + 1))
  // We want Baudrate = 115200
  // We know System Clock Speed = 250,000,000
  // From the formula, we get Baud = 270
  set(AUX_MU_BAUD, 270);

  // Enable the UART transmitter. We should be good to go at this point.
  set(AUX_MU_CNTL_REG,2);

  // Turn Green ON
  GPIO_set(11, GPIO_LEV_HIGH);
  
  // Start sending data
  int count =0;
  while(1){
  	while(1){
		if(get(AUX_MU_LSR_REG)&0x20) break;
	}
	set(AUX_MU_IO_REG,0x30+(count++&7));
  }

  while(1) {
  }
  return 0;
}
