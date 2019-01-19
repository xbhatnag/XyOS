.section ".text"

.global enable_irq
enable_irq:
	// Unmask IRQ bit in DAIF
	msr DAIFClr, #15
	ret
