.global launch_kernel
launch_kernel:
	// Initialize system control registers for EL2
	ldr x0, =0xC5183E
	msr SCTLR_EL2, x0

	// Mark EL1 execution state as AArch64
	mov x0, #0x0
	orr x0, x0, #(1<<31)
	msr HCR_EL2, x0

	// Mark exception source as EL1h.
	MOV X0, #0b00101
	MSR SPSR_EL2, X0

	// EL1 Stack Pointer is the Scheduler Stack
	ldr x0, =0xFFFFFFFFFF000000
	msr SP_EL1, x0

	// Set exception return address
	ldr x0, =0xFFFF0000000E0000
	msr ELR_EL2, x0

	// Clear TLB
	TLBI ALLE1

	// Wait for all data accesses
	DSB SY

	// Wait for all instructions
	ISB SY

	// Go to EL1
	eret
