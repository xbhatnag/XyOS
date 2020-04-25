.section ".text"

.global irq_enable
irq_enable:
	msr DAIFClr, #15
	ret

.global irq_disable
irq_disable:
	msr DAIFSet, #15
	ret

.global interrupt_status_1
interrupt_status_1:
	mrs x0, ISR_EL1
	ret

.global exception_syndrome_2
exception_syndrome_2:
	mrs x0, ESR_EL2
	ret

.global exception_link_2
exception_link_2:
	mrs x0, ELR_EL2
	ret

.global saved_program_state_2
saved_program_state_2:
	mrs x0, SPSR_EL2
	ret

.global exception_syndrome_1
exception_syndrome_1:
	mrs x0, ESR_EL1
	ret

.global exception_link_1
exception_link_1:
	mrs x0, ELR_EL1
	ret

.global saved_program_state_1
saved_program_state_1:
	mrs x0, SPSR_EL1
	ret

.global exception_handler
exception_handler:
	// Disable all H/W interrupts.
	// It's hard to deal with another
	// H/W interrupt while this one is
	// being dealt with.
	msr DAIFSet, #15

	// Wait for all data accesses
	DSB SY

	// Wait for all instructions
	ISB SY

	// Save all registers
	sub sp, sp, #16
	stp x0,x1,[sp]

	sub sp, sp, #16
	stp x2,x3,[sp]

	sub sp, sp, #16
	stp x4,x5,[sp]

	sub sp, sp, #16
	stp x6,x7,[sp]

	sub sp, sp, #16
	stp x8,x9,[sp]

	sub sp, sp, #16
	stp x10,x11,[sp]

	sub sp, sp, #16
	stp x12,x13,[sp]

	sub sp, sp, #16
	stp x14,x15,[sp]

	sub sp, sp, #16
	stp x16,x17,[sp]

	sub sp, sp, #16
	stp x18,x19,[sp]

	sub sp, sp, #16
	stp x20,x21,[sp]

	sub sp, sp, #16
	stp x22,x23,[sp]

	sub sp, sp, #16
	stp x24,x25,[sp]

	sub sp, sp, #16
	stp x26,x27,[sp]

	sub sp, sp, #16
	stp x28,x29,[sp]

	sub sp, sp, #16
	stp x30,x0,[sp]

	// Store return address for the current thread
	mrs x0, ELR_EL1
	ldr x1, =current_thread_saved_program_counter
	str x0, [x1, 0]

	// Store stack pointer for the current thread
	mov x0, sp
	ldr x1, =current_thread_saved_stack_pointer
	str x0, [x1, 0]

	// Set stack to the scheduler stack
	ldr x0, =0xFFFFFFFFFF000000
	add sp, x0, #0

	// Go to EL1 exception handler
	bl exception_handler_1

	// Wait for all data accesses
	DSB SY

	// Wait for all instructions
	ISB SY

	// Restore return address for the current thread
	ldr x0, =current_thread_saved_program_counter
	ldr x1, [x0, 0]
	msr ELR_EL1, x1

	// Restore stack pointer for the current thread
	ldr x0, =current_thread_saved_stack_pointer
	ldr x1, [x0, 0]
	add sp, x1, #0

restore_registers:
	ldp x30,x0,[sp]
	add sp, sp, #16

	ldp x28,x29,[sp]
	add sp, sp, #16

	ldp x26,x27,[sp]
	add sp, sp, #16

	ldp x24,x25,[sp]
	add sp, sp, #16

	ldp x22,x23,[sp]
	add sp, sp, #16

	ldp x20,x21,[sp]
	add sp, sp, #16

	ldp x18,x19,[sp]
	add sp, sp, #16

	ldp x16,x17,[sp]
	add sp, sp, #16

	ldp x14,x15,[sp]
	add sp, sp, #16

	ldp x12,x13,[sp]
	add sp, sp, #16

	ldp x10,x11,[sp]
	add sp, sp, #16

	ldp x8,x9,[sp]
	add sp, sp, #16

	ldp x6,x7,[sp]
	add sp, sp, #16

	ldp x4,x5,[sp]
	add sp, sp, #16

	ldp x2,x3,[sp]
	add sp, sp, #16

	ldp x0,x1,[sp]
	add sp, sp, #16

	// Re-enable H/W interrupts
	msr DAIFClr, #15

	// Wait for all data accesses
	DSB SY

	// Wait for all instructions
	ISB SY

	// Return from exception
	// At this point, ELR_ELn
	// should be set correctly
	eret
