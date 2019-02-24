.section ".text.boot"

_boot:
    // get cpu id
    mrs     x1, mpidr_el1
    and     x1, x1, #3

    // jump if cpu id == 0
    cbz     x1, 2f

    // infinite loop
1:  wfe
    b       1b

    // cpu id == 0
    // clear bss
    // TODO: Why is x1 not incremented?
2:  ldr     x1, =__bss_start
    ldr     w2, =__bss_size
3:  cbz     w2, 4f
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b

    // jump to C code
4:  bl      main
    // just in case we somehow return...
    b       1b
