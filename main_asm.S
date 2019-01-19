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

.global interrupt_status_1
interrupt_status_1:
	mrs x0, ISR_EL1
	ret
