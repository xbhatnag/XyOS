#ifndef __VMEM_H__
#define __VMEM_H__

#include "Peripherals.h"

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
#define PAGE_MAP_START_ADDRESS          ((volatile char*) 0x0)
#define FREE_PAGE                       'F'
#define USED_PAGE                       'U'

void vmem_init();

#endif
