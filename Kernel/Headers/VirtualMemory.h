#ifndef __VMEM_H__
#define __VMEM_H__

#include <stdint.h>

#define TOP_OF_MEMORY					0xFFFF000000000000
#define PAGE_SIZE                       KB_64
#define KB_64                           (64 * 1024)
#define MB_512                          (512 * 1024 * 1024)
#define MASK_PHYS_ADDR                  0xFFFFFFFF0000
#define TABLE_DESC_ID                   0x3
#define NORMAL_BLOCK_DESC_ID            0x403
#define DEVICE_BLOCK_DESC_ID            0x407
#define L1_TOTAL_PAGES                  64
#define L2_TOTAL_PAGES                  8192
#define L3_TOTAL_PAGES                  8192
#define L1_TABLE_SIZE                   (8 * L1_TOTAL_PAGES)
#define L2_TABLE_SIZE                   (8 * L2_TOTAL_PAGES)
#define L3_TABLE_SIZE                   (8 * L2_TOTAL_PAGES)
#define PERIPHERALS_PAGES               256

void vmem_init();
uintptr_t allocate_from_lower_l3(uintptr_t paddr, uint32_t len);

#endif
