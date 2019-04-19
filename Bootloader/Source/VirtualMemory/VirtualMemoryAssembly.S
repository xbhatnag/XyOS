// Enable M bit in SCTLR_EL1
// Disable VM bit in HCR_EL2
// Figure out bits for AS, IPS, TG1, T1SZ, TG0, EPD1, EPD0, T0SZ in TCR_EL1

// 2 address ranges for translation (user and kernel space)
// 1 stage of translation controlled by EL2 and EL1
// EL2 sets up everything for EL1 so that EL1 also lives in virtual memory
// EL1 can change user address range for different processes.

.global set_translation_base_register_1
set_translation_base_register_1:
	msr TTBR1_EL1, x0
	ret

.global enable_translation
enable_translation:
	// Mark EL1 execution state as AArch64
	mov x0, #0x0
	orr x0, x0, #(1<<31)
	msr HCR_EL2, x0

	// Set TCR_EL1 and enable TTBR1 translation
	ldr x0, =0xC0100080
	msr TCR_EL1, x0

	// MAIR_EL1 Config
	// Attr0 -> Normal, Non-cacheable Memory
	// Attr1 -> Device, nGnRnE Memory
	ldr x0, =0x44
	msr MAIR_EL1, x0

	// Set M bit (MMU)
	// Set C bit (data cacheability)
	// Set I bit (instruction cacheability)
	ldr x0, =0xC01805
	msr SCTLR_EL1, x0

	// Clear the TLB
	TLBI ALLE1

	// Synchronize on Data and Instruction
	DSB SY
	ISB

	ret

.global test_translation
test_translation:
	AT S1E1R, x0
	DSB SY
	ISB
	mrs x0, PAR_EL1
	ret
