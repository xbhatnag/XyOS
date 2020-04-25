.section ".text.boot"

_boot:
    // Zero out all registers
    MOV    X0, XZR
    MOV    X1, XZR
    MOV    X2, XZR
    MOV    X3,  XZR
    MOV    X4,  XZR
    MOV    X5,  XZR
    MOV    X6,  XZR
    MOV    X7,  XZR
    MOV    X8,  XZR
    MOV    X9,  XZR
    MOV    X10, XZR
    MOV    X11, XZR
    MOV    X12, XZR
    MOV    X13, XZR
    MOV    X14, XZR
    MOV    X15, XZR
    MOV    X16, XZR
    MOV    X17, XZR
    MOV    X18, XZR
    MOV    X19, XZR
    MOV    X20, XZR
    MOV    X21, XZR
    MOV    X22, XZR
    MOV    X23, XZR
    MOV    X24, XZR
    MOV    X25, XZR
    MOV    X26, XZR
    MOV    X27, XZR
    MOV    X28, XZR
    MOV    X29, XZR
    MOV    X30, XZR

    // get cpu id
    mrs     x1, mpidr_el1
    and     x1, x1, #3

    // jump if cpu id == 0
    cbz     x1, 2f

    // infinite loop if cpu id != 0
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
