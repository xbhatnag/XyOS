#include "VirtualMemory.h"
#include "ConsoleIO.h"
#include "Mailbox.h"
#include "Exceptions.h"

extern uint64_t __kernel_start, __kernel_end;

uintptr_t kernel_start = (uintptr_t) &__kernel_start;
uintptr_t kernel_end = (uintptr_t) &__kernel_end;

uintptr_t kernel_l1_page_table_begin;
uintptr_t kernel_l1_page_table_end;

uintptr_t kernel_upper_l2_page_table_begin;
uintptr_t kernel_upper_l2_page_table_end;

uintptr_t kernel_upper_l3_page_table_begin;
uintptr_t kernel_upper_l3_page_table_current;
uintptr_t kernel_upper_l3_page_table_end;

uintptr_t kernel_lower_l2_page_table_begin;
uintptr_t kernel_lower_l2_page_table_end;

uintptr_t kernel_lower_l3_page_table_begin;
uintptr_t kernel_lower_l3_page_table_current;
uintptr_t kernel_lower_l3_page_table_end;

uintptr_t kernel_stack_top;
uintptr_t kernel_stack_bottom;

uintptr_t align_64_kb(uintptr_t address) {
	uint64_t remainder = address % KB_64;
	uint64_t aligned = address - remainder;
	if (remainder) aligned += KB_64;
	return aligned;
}

uint64_t create_table_descriptor(uintptr_t address) {
	return (address & MASK_PHYS_ADDR) | TABLE_DESC_ID;
}

uint64_t create_normal_block_descriptor(uintptr_t address) {
	return (address & MASK_PHYS_ADDR) | NORMAL_BLOCK_DESC_ID;
}

uint64_t create_device_block_descriptor(uintptr_t address) {
	return (address & MASK_PHYS_ADDR) | DEVICE_BLOCK_DESC_ID;
}

void measure_page_tables() {
	// The linker gives us the address of the end of the kernel.
	kernel_l1_page_table_begin = align_64_kb(kernel_end);
	kernel_l1_page_table_end = kernel_l1_page_table_begin + L1_TABLE_SIZE;

	// Reserve space for upper 4TB L2 Page Table
	kernel_upper_l2_page_table_begin = align_64_kb(kernel_l1_page_table_end);
	kernel_upper_l2_page_table_end = kernel_upper_l2_page_table_begin + L2_TABLE_SIZE;

	// Reserve space for upper 512 MB L3 Page Table
	kernel_upper_l3_page_table_begin = align_64_kb(kernel_upper_l2_page_table_end);
	kernel_upper_l3_page_table_end = kernel_upper_l3_page_table_begin + L3_TABLE_SIZE;

	// Reserve space for lower 4TB L2 Page Table
	kernel_lower_l2_page_table_begin = align_64_kb(kernel_upper_l3_page_table_end);
	kernel_lower_l2_page_table_end = kernel_lower_l2_page_table_begin + L2_TABLE_SIZE;

	// Reserve space for lower 512 MB L3 Page Table
	kernel_lower_l3_page_table_begin = align_64_kb(kernel_lower_l2_page_table_end);
	kernel_lower_l3_page_table_end = kernel_lower_l3_page_table_begin + L3_TABLE_SIZE;

	// Reserve 64KB for Kernel Stack
	kernel_stack_top = align_64_kb(kernel_lower_l3_page_table_end);
	kernel_stack_bottom = kernel_stack_top + KB_64;

	uint32_t num_pages_used = kernel_stack_bottom / PAGE_SIZE;
	if (kernel_stack_bottom % PAGE_SIZE) {
		num_pages_used += 1;
	}

	println("   ╔═══════════════════════════════════════════════════════╗");
	puth_with_title_64("   ║ Kernel Start Address             = ", kernel_start);
	puth_with_title_64("   ║ Kernel End Address               = ", kernel_end);
	puth_with_title_64("   ║ Kernel L1 Page Table Begin       = ", kernel_l1_page_table_begin);
	puth_with_title_64("   ║ Kernel L1 Page Table End         = ", kernel_l1_page_table_end);
	puth_with_title_64("   ║ Kernel Upper L2 Page Table Begin = ", kernel_upper_l2_page_table_begin);
	puth_with_title_64("   ║ Kernel Upper L2 Page Table End   = ", kernel_upper_l2_page_table_end);
	puth_with_title_64("   ║ Kernel Upper L3 Page Table Begin = ", kernel_upper_l3_page_table_begin);
	puth_with_title_64("   ║ Kernel Upper L3 Page Table End   = ", kernel_upper_l3_page_table_end);
	puth_with_title_64("   ║ Kernel Lower L2 Page Table Begin = ", kernel_lower_l2_page_table_begin);
	puth_with_title_64("   ║ Kernel Lower L2 Page Table End   = ", kernel_lower_l2_page_table_end);
	puth_with_title_64("   ║ Kernel Lower L3 Page Table Begin = ", kernel_lower_l3_page_table_begin);
	puth_with_title_64("   ║ Kernel Lower L3 Page Table End   = ", kernel_lower_l3_page_table_end);
	puth_with_title_64("   ║ Kernel Stack Top                 = ", kernel_stack_top);
	puth_with_title_64("   ║ Kernel Stack Bottom              = ", kernel_stack_bottom);
	println("   ╚═══════════════════════════════════════════════════════╝");
}

void measure_kernel_physical_memory() {
	uint64_t num_kernel_pages = (kernel_lower_l3_page_table_end - TOP_OF_MEMORY) / PAGE_SIZE;

	if (kernel_lower_l3_page_table_end % PAGE_SIZE) {
		num_kernel_pages += 1;
	}

	kernel_upper_l3_page_table_current = kernel_upper_l3_page_table_begin + (num_kernel_pages * 8);
}

void measure_peripherals_and_kernel_stack() {
	// How many pages used
	uint64_t num_pages_skipped = L3_TOTAL_PAGES - (PERIPHERALS_PAGES + 1);
	kernel_lower_l3_page_table_current = kernel_lower_l3_page_table_begin + (num_pages_skipped * 8);
}

uintptr_t allocate_from_lower_l3(uintptr_t paddr, uint32_t len) {
	if (paddr != align_64_kb(paddr)) {
		error("Give aligned addresses when allocating!");
	}

	// Calculate the physical address of the last page
	uintptr_t paddr_cur = (paddr + len) & MASK_PHYS_ADDR;

	// Work backwards to the first page
	while(paddr_cur >= paddr) {
		kernel_lower_l3_page_table_current -= 8;
		*((uint64_t*)(kernel_lower_l3_page_table_current)) = create_normal_block_descriptor(paddr_cur);
		paddr_cur -= PAGE_SIZE;
	}

	uint64_t num_used_pages_in_lower_l3 = (kernel_lower_l3_page_table_end - kernel_lower_l3_page_table_current) >> 3;

	return LAST_PAGE_VADDR - ((num_used_pages_in_lower_l3 - 1) * PAGE_SIZE);
}

void vmem_init() {
	measure_page_tables();
	measure_kernel_physical_memory();
	measure_peripherals_and_kernel_stack();
}
