.section .text
.global _start

_start:
    addi sp, sp, -16
    sw a0, 12(sp)
    sw a1, 8(sp)
    li a0, 5
    li a1, 10
    call add_two_num # equal to "add_two_num(5, 10)" in C code
    tail exit
add_two_num:
    add a0, a0, a1
    ret
exit:
    # done...
