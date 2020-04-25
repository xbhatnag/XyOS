SECTIONS
{
    . = 0xFFFF0000000E0000;
    __start = .;
    .text : { KEEP(*(.text.boot)) *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss)
        __bss_end = .;
    }
    __end = .;

   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}
__bss_size = (__bss_end - __bss_start)>>3;
