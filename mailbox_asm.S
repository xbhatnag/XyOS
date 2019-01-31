.section ".text"
.global mailbox_write
mailbox_write:
	mailbox .req x4			// Create an alias
	ldr mailbox, =0x3f00b880	// Mailbox Base Address
mailbox_write_loop:
	ldr x5, [mailbox, #0x18]	// Get status register
	tst x5, #0x80000000		// Is the full flag set?
	bne mailbox_write_loop		// Wait if flag is set
	
	// Assume x0 holds the address to the structure.
	// Assume this structure is 4-byte aligned 
	add x0, x0, #0x8		// Add the channel number (8)
	str x0, [mailbox, #0x20]	// Put the data into the write register
	
	.unreq mailbox			// Remove alias
	ret				// Return

.global mailbox_wait_read
mailbox_wait_read:
	mailbox .req x4			// Create an alias
	ldr mailbox, =0x3f00b880	// Mailbox Base Address
mailbox_wait_read_loop:
	ldr x5, [mailbox, #0x18]	// Get status register
	tst x5, #0x40000000		// Is the empty flag set?
	bne mailbox_wait_read_loop	// Wait if flag is set
	
	ldr x5, [mailbox]		// Get read register
	tst x5, #8			// Is channel 8?
	beq mailbox_wait_read_loop	// Wait if not channel 8

	.unreq mailbox			// Remove alias
	ret				// Return
