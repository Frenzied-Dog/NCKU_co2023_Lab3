OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS {
    . = 0x0000;
    .text : {*(.text._start) *(.text)}
    . = 0x8000;
    PROVIDE (__global_pointer$  = . + 0x800);
    .data : {*(.rodata) *(.data)}
    . = 0x9000;
    _answer = .;
    . = 0xffff;
    _stack = .;
    PROVIDE (__stack_top = .);
}