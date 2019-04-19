.section ".text"

.global variant
variant:
	mrs x0, MIDR_EL1
	and x0, x0, #0x100000	// Only bits (23-20) are relevant
	lsr x0, x0, #20		// Shift those bits right
	ret

.global implementer
implementer:
	mrs x0, MIDR_EL1
	lsr x0, x0, #24		// Only bits (31-24) are relevant
	ret

.global exception_level
exception_level:
	mrs x0, CurrentEL
	and x0, x0, #12		// Only bits 3 and 2 are relevant
	lsr x0, x0, #2		// Shift right by 2
	ret

.global get_stack_ptr
get_stack_ptr:
	mov x0, sp
	ret
