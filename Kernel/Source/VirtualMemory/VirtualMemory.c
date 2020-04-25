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
uint32_t kernel_upper_l3_page_table_num_pages_used;
uintptr_t kernel_upper_l3_page_table_end;

uintptr_t kernel_lower_l2_page_table_begin;
uintptr_t kernel_lower_l2_page_table_end;

uintptr_t kernel_lower_l3_page_table_begin;
uintptr_t kernel_lower_l3_page_table_current;
uint32_t kernel_lower_l3_page_table_num_pages_used;
uintptr_t kernel_lower_l3_page_table_end;

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
	// TODO: The linker reports a different end of kernel than what
	//       was calculated by the bootloader. Off by approximately 0xf8 bytes.
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

	println(           "   #########################################################");
	puth_with_title_64("   # Kernel Start Address             = ", kernel_start);
	puth_with_title_64("   # Kernel End Address               = ", kernel_end);
	puth_with_title_64("   # Kernel L1 Page Table Begin       = ", kernel_l1_page_table_begin);
	puth_with_title_64("   # Kernel L1 Page Table End         = ", kernel_l1_page_table_end);
	puth_with_title_64("   # Kernel Upper L2 Page Table Begin = ", kernel_upper_l2_page_table_begin);
	puth_with_title_64("   # Kernel Upper L2 Page Table End   = ", kernel_upper_l2_page_table_end);
	puth_with_title_64("   # Kernel Upper L3 Page Table Begin = ", kernel_upper_l3_page_table_begin);
	puth_with_title_64("   # Kernel Upper L3 Page Table End   = ", kernel_upper_l3_page_table_end);
	puth_with_title_64("   # Kernel Lower L2 Page Table Begin = ", kernel_lower_l2_page_table_begin);
	puth_with_title_64("   # Kernel Lower L2 Page Table End   = ", kernel_lower_l2_page_table_end);
	puth_with_title_64("   # Kernel Lower L3 Page Table Begin = ", kernel_lower_l3_page_table_begin);
	puth_with_title_64("   # Kernel Lower L3 Page Table End   = ", kernel_lower_l3_page_table_end);
	println(           "   #########################################################");
}

void measure_top_of_physical_memory() {
	uint64_t top_of_physical_memory_num_bytes_used = kernel_lower_l3_page_table_end - TOP_OF_MEMORY;
	kernel_upper_l3_page_table_num_pages_used = top_of_physical_memory_num_bytes_used / PAGE_SIZE;

	if (top_of_physical_memory_num_bytes_used % PAGE_SIZE) {
		kernel_upper_l3_page_table_num_pages_used += 1;
	}

	kernel_upper_l3_page_table_current = kernel_upper_l3_page_table_begin + (kernel_upper_l3_page_table_num_pages_used * 8);
}

void measure_peripherals_and_scheduler_stack() {
	// 1 page used for scheduler stack
	kernel_lower_l3_page_table_num_pages_used = PERIPHERALS_PAGES + 1;
	uint64_t num_pages_skipped = L3_TOTAL_PAGES - kernel_lower_l3_page_table_num_pages_used;
	kernel_lower_l3_page_table_current = kernel_lower_l3_page_table_begin + (num_pages_skipped * 8);
}

uintptr_t allocate_from_lower_l3(uintptr_t paddr_start, uint32_t len) {
	if (paddr_start & 0xFFFF) {
		error("Give aligned addresses when allocating!");
	}

	uint32_t num_pages_required = len / PAGE_SIZE;
	if (len % PAGE_SIZE) {
		num_pages_required += 1;
	}

	// Start from the last page that needs to be allocated
	uintptr_t paddr_cur = paddr_start + ((num_pages_required - 1) * PAGE_SIZE);
	uintptr_t last_page_paddr = paddr_cur;

	while(paddr_cur >= paddr_start) {
		// Move the pointer to the current page table entry up by one.
		kernel_lower_l3_page_table_current -= 8;

		// Make an entry for the current physical address
		*((uint64_t*)(kernel_lower_l3_page_table_current)) = create_normal_block_descriptor(paddr_cur);
		
		// Increment number of pages used
		kernel_lower_l3_page_table_num_pages_used += 1;

		// Shift the current physical address up by a page
		paddr_cur -= PAGE_SIZE;
	}

	// TODO: It is possible to overflow the L3 page table. Prevent that from happening!

	// TODO: This math is hacky and I don't like it. Find a better way!									
	uintptr_t vaddr_start = 0xFFFFFFFFFFFF0000 - ((kernel_lower_l3_page_table_num_pages_used - 1) * PAGE_SIZE);

	println(           "   #####################################################");
	puti_with_title_32("   # Allocation Size            = ", len);
	puth_with_title_64("   # Physical Address Start     = ", paddr_start);
	puti_with_title_32("   # Number of Pages Required   = ", num_pages_required);
	puth_with_title_64("   # Last Page Physical Address = ", last_page_paddr);
	puth_with_title_64("   # Virtual Address Start      = ", vaddr_start);
	println(           "   #####################################################");

	return vaddr_start;
}

void vmem_init() {
	measure_page_tables();
	measure_top_of_physical_memory();
	measure_peripherals_and_scheduler_stack();
}
