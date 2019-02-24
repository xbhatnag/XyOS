.section ".text"

.global enable_irq
enable_irq:
	msr DAIFClr, #15
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

	// Jump to right exception handler
	mrs x0, CurrentEL
	and x0, x0, #12		// Only bits 3 and 2 are relevant
	lsr x0, x0, #2		// Shift right by 2

is_el2:
	// Check if EL2, fail otherwise
	cmp x0, #2
	bne generic_error

	// Go to exception handler 2
	bl exception_handler_2
	b restore_registers

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

	// Return from exception
	// At this point, ELR_ELn
	// should be set correctly
	eret
