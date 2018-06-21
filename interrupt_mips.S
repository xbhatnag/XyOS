#include "vectors.S"

.section ".text"

.global clear_all_daif
clear_all_daif:
	msr DAIFClr, #0xf	// Clear all bit masks
	ret

.global set_vector_table
set_vector_table:
	ldr x0, = el2_vector_table
	msr VBAR_EL2, x0
	ret

.global set_secure_config
set_secure_config:
	mrs x0, HCR_EL2
	orr x0, x0, #(0x3f<<3) 	// Virtual + Physical IRQ, FIQ, Async abort and System Error
	msr HCR_EL2, x0
	ret

.global exception_class
exception_class:
	mrs x0, ESR_EL2
	lsr x0, x0, #26
	ret

.global instruction_length
instruction_length:
	mrs x0, ESR_EL2
	lsr x0, x0, #25
	and x0, x0, #1
	ret

.global exception_syndrome
exception_syndrome:
	mrs x0, ESR_EL2
	ret

.global fault_address
fault_address:
	mrs x0, FAR_EL2
	ret

