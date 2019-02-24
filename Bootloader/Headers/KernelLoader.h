#ifndef _KERNEL_LOADER_H_
#define _KERNEL_LOADER_H_

#include <stdint.h>

#define KERNEL_PHYSICAL_START 0xE0000
extern uint64_t kernel_size;

void download_kernel();
void launch_kernel();

#endif
