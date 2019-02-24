.section ".text"

.global set_vector_table
set_vector_table:
	adr x0, vector_table
	msr VBAR_EL1, x0
	ret

.balign 0x800
vector_table:
synchronous_curr_sp0:
	b generic_error
	.balign 0x80
irq_el2_sp0:
	b generic_error
	.balign 0x80
fiq_curr_sp0:
	b generic_error
	.balign 0x80
serror_curr_sp0:
	b generic_error
	.balign 0x80
synchronous_curr_spcurr:
	b exception_handler
	.balign 0x80
irq_curr_spcurr:
	b exception_handler
	.balign 0x80
fiq_curr_spcurr:
	b generic_error
	.balign 0x80
serror_curr_spcurr:
	b generic_error
	.balign 0x80
synchronous_lower_64:
	b exception_handler
	.balign 0x80
irq_lower_64:
	b generic_error
	.balign 0x80
fiq_lower_64:
	b generic_error
	.balign 0x80
serror_lower_64:
	b generic_error
	.balign 0x80
synchronous_lower_32:
	b generic_error
	.balign 0x80
irq_lower_32:
	b generic_error
	.balign 0x80
fiq_lower_32:
	b generic_error
	.balign 0x80
serror_lower_32:
	b generic_error
	.balign 0x80
