#include "VirtualMemory.h"
#include "ConsoleIO.h"
#include "KernelLoader.h"
#include "Mailbox.h"
#include "Exceptions.h"

uintptr_t kernel_l1_page_table_begin;
uintptr_t kernel_l1_page_table_end;

uintptr_t kernel_upper_l2_page_table_begin;
uintptr_t kernel_upper_l2_page_table_end;

uintptr_t kernel_upper_l3_page_table_begin;
uintptr_t kernel_upper_l3_page_table_end;

uintptr_t kernel_lower_l2_page_table_begin;
uintptr_t kernel_lower_l2_page_table_end;

uintptr_t kernel_lower_l3_page_table_begin;
uintptr_t kernel_lower_l3_page_table_end;

uintptr_t kernel_stack_top;
uintptr_t kernel_stack_bottom;

extern void set_translation_base_register_1(uintptr_t base_reg);
extern void enable_translation();
extern uint64_t test_translation(uint64_t value);

void print_val_32(char* title, uint32_t val) {
	print(title);
	puti_32(val);
	newline();
}

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

void reserve_page_tables() {
	// L1 : 5 bits. 64 entries. Each entry points to 4TB L2 Table
	// L2 : 13 bits. 8192 entries. Each entry points to 512 MB Table
	// L3 : 13 bits. 8192 entries. Each entry points to 16 KB Block

	// Each L1 entry corresponds to a 4TB block
	// Each L2 entry corresponds to a 512MB block
	// Each L3 entry corresponds to a 64KB block

	// 1 L1 table with 1 L2 table entry
	// 1 L2 table with 2 L3 table entries
	// 2 L3 tables with 8192 block entries

	// There is 1 L1 table which goes into TTBRx_ELx
	// There is 1 L2 table because nobody has more than 4GB of RAM
	// We need 2x the tables because there are 2 address spaces - User and Kernel

	/**
	0x0000000000000000
	Bootloader Stack
	0x00000000000C0000
	Bootloader
	0x00000000000E0000
	Kernel (assume size 0x10000)
	0x00000000000F0000
	Kernel L1 Page Table
	0x00000000000F0200
	Empty Space
	0x0000000000100000
	Kernel Upper L2 Page Table
	0x0000000000110000
	Kernel Upper L3 Page Table
	0x0000000000120000
	Kernel Lower L2 Page Table
	0x0000000000130000
	Kernel Lower L3 Page Table
	0x0000000000140000
	Kernel stack
	0x0000000000150000
	**/

	uintptr_t kernel_physical_end = KERNEL_PHYSICAL_START + kernel_size;

	kernel_l1_page_table_begin = align_64_kb(kernel_physical_end);
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

	puth_with_title_64("Kernel Start Address             = ", KERNEL_PHYSICAL_START);
	puth_with_title_64("Kernel End Address               = ", kernel_physical_end);
	puth_with_title_64("Kernel L1 Page Table Begin       = ", kernel_l1_page_table_begin);
	puth_with_title_64("Kernel L1 Page Table End         = ", kernel_l1_page_table_end);
	puth_with_title_64("Kernel Upper L2 Page Table Begin = ", kernel_upper_l2_page_table_begin);
	puth_with_title_64("Kernel Upper L2 Page Table End   = ", kernel_upper_l2_page_table_end);
	puth_with_title_64("Kernel Upper L3 Page Table Begin = ", kernel_upper_l3_page_table_begin);
	puth_with_title_64("Kernel Upper L3 Page Table End   = ", kernel_upper_l3_page_table_end);
	puth_with_title_64("Kernel Lower L2 Page Table Begin = ", kernel_lower_l2_page_table_begin);
	puth_with_title_64("Kernel Lower L2 Page Table End   = ", kernel_lower_l2_page_table_end);
	puth_with_title_64("Kernel Lower L3 Page Table Begin = ", kernel_lower_l3_page_table_begin);
	puth_with_title_64("Kernel Lower L3 Page Table End   = ", kernel_lower_l3_page_table_end);
	puth_with_title_64("Kernel Stack Top                 = ", kernel_stack_top);
	puth_with_title_64("Kernel Stack Bottom              = ", kernel_stack_bottom);
}

void create_table_entries() {
	println("Creating Upper L2 Table Entry in L1 table...");
	*((uint64_t*)kernel_l1_page_table_begin) = create_table_descriptor(kernel_upper_l2_page_table_begin);

	println("Creating Lower L2 Table Entry in L1 table...");
	*((uint64_t*)(kernel_l1_page_table_end - 8)) = create_table_descriptor(kernel_lower_l2_page_table_begin);

	println("Creating Upper L3 Table Entry in Upper L2 table...");
	*((uint64_t*)kernel_upper_l2_page_table_begin) = create_table_descriptor(kernel_upper_l3_page_table_begin);

	println("Creating Lower L3 Table Entry in Lower L2 table...");
	*((uint64_t*)(kernel_lower_l2_page_table_end - 8)) = create_table_descriptor(kernel_lower_l3_page_table_begin);
}

void map_kernel_physical_memory() {
	// Map the entire top of physical memory into virtual memory,
	// upto and including the end of the last l3 page table.
	uint64_t num_kernel_pages = (kernel_lower_l3_page_table_end - TOP_OF_MEMORY) / PAGE_SIZE;

	if (kernel_lower_l3_page_table_end % PAGE_SIZE) {
		num_kernel_pages += 1;
	}

	print("Mapping ");
	puti_64(num_kernel_pages);
	print(" pages of kernel physical memory...");
	newline();

	uintptr_t physical_address = 0x0;
	uintptr_t current_l3_page_table_entry = kernel_upper_l3_page_table_begin;

	for (uint64_t i = 0; i < num_kernel_pages; i++) {
		*((uint64_t*)current_l3_page_table_entry) = create_normal_block_descriptor(physical_address);
		physical_address += PAGE_SIZE;
		current_l3_page_table_entry += 8;
	}
}

void map_peripherals_and_kernel_stack() {
	// How many pages to skip?
	uint64_t num_pages_to_skip = L3_TOTAL_PAGES - PERIPHERALS_PAGES - 1;
	print("Skipping ");
	puti_64(num_pages_to_skip);
	println(" pages of lower L3 table");
	uintptr_t current_l3_page_table_entry = kernel_lower_l3_page_table_begin + (num_pages_to_skip * 8);

	// Map Kernel Stack
	println("Mapping 1 page of kernel stack...");
	*((uint64_t*)(current_l3_page_table_entry)) = create_normal_block_descriptor(kernel_stack_top);
	current_l3_page_table_entry += 8;

	// Map Peripherals
	print("Mapping ");
	puti_64(PERIPHERALS_PAGES);
	println(" pages of peripherals...");

	uintptr_t physical_address = PERIPHERALS_BASE;

	for (uint64_t i = 0; i < PERIPHERALS_PAGES; i++) {
		*((uint64_t*)current_l3_page_table_entry) = create_device_block_descriptor(physical_address);
		physical_address += PAGE_SIZE;
		current_l3_page_table_entry += 8;
	}
}

void vmem_init() {
	reserve_page_tables();
	create_table_entries();
	map_kernel_physical_memory();
	map_peripherals_and_kernel_stack();
	set_translation_base_register_1(kernel_l1_page_table_begin);
	enable_translation();
	puth_with_title_64("Test TTBR1 Memory   = ", test_translation(0xFFFF000000000000));
	puth_with_title_64("Test Kernel Code    = ", test_translation(0xFFFF0000000E0000));
	puth_with_title_64("Test Invalid Access = ", test_translation(0xFFFFDDDDDDDDDDDD));
	puth_with_title_64("Test Kernel Stack   = ", test_translation(0xFFFFFFFFFEFF0000));
	puth_with_title_64("Test Kernel Stack   = ", test_translation(0xFFFFFFFFFEFFFFFF));
	puth_with_title_64("Test Peripherals    = ", test_translation(0xFFFFFFFFFF000000));
	puth_with_title_64("Test Peripherals    = ", test_translation(0xFFFFFFFFFFFFFFFF));
}
