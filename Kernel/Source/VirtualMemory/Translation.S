.section ".text"

.global get_physical_address
get_physical_address:
    AT S1E1R, x0
    mrs x1, PAR_EL1
    ldr x2, =0xfffffffff000
    and x1, x1, x2
    ldr x2, =0xfff
    and x0, x0, x2
    add x0, x0, x1
    ret

.global test_translation
test_translation:
	AT S1E1R, x0
	DSB SY
	ISB
	mrs x0, PAR_EL1
	ret