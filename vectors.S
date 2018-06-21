.section ".text"

.balign 0x800
el2_vector_table:
synchronous_el2_sp0:
	bl exception_handler
	.balign 0x80
irq_el2_sp0:
	bl exception_handler
	.balign 0x80
fiq_el2_sp0:
	bl exception_handler
	.balign 0x80
serror_el2_sp0:
	bl exception_handler
	.balign 0x80
synchronous_el2_sp2:
	bl exception_handler
	.balign 0x80
irq_el2_sp2:
	bl exception_handler
	.balign 0x80
fiq_el2_sp2:
	bl exception_handler
	.balign 0x80
serror_el2_sp2:
	bl exception_handler
	.balign 0x80
synchronous_not_el2_64:
	bl exception_handler
	.balign 0x80
irq_not_el2_64:
	bl exception_handler
	.balign 0x80
fiq_not_el2_64:
	bl exception_handler
	.balign 0x80
serror_not_el2_64:
	bl exception_handler
	.balign 0x80
synchronous_not_el2_32:
	bl exception_handler
	.balign 0x80
irq_not_el2_32:
	bl exception_handler
	.balign 0x80
fiq_not_el2_32:
	bl exception_handler
	.balign 0x80
serror_not_el2_32:
	bl exception_handler
	.balign 0x80
