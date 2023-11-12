    .section .data
case_1:
    .word -1, 1, 4, -1, -1, 3, -1, 2, -1, -1, 3, -1, -1, -1, -1, -1
case_2:
    .word 2, -1, -1, -1, -1, -1, 1, -1, -1, 2, -1, -1, -1, -1, -1, 4
case_3:
    .word -1, 2, 3, -1, 1, -1, -1, 4, 2, -1, -1, 3, -1, 1, 4, -1

    .section .text
    .global prog
prog:
    la a0, case_2
    li a1, 0
    li a2, 0
    call solver

    la a0, fmt1
    la t3, case_2
    lw a1, 0(t3)
    call printf

    li t4, 16
    li t1, 1

    print_puzzle_l:
        slli t2, t1, 2
        la t3, case_2
        add t3, t3, t2
        la a0, fmt2
        lw a1, 0(t3)
        call printf

        addi t1, t1, 1
        bne t1, t4, print_puzzle_l
    tail exit

# s1 = nRow              s2 = nCol
# s3 = num
# s4: board + row*4+col  s5: board
# s6 = row               s7 = col
# t1: board[row * 4 + col]
# t5 = -1                t6 = 4
solver:
    addi sp, sp, -32
    sw s7, 28(sp)
    sw s6, 24(sp)
    sw s5, 20(sp)
    sw s4, 16(sp)
    sw s3, 12(sp)
    sw s2, 8(sp) 
    sw s1, 4(sp)
    sw ra, 0(sp)

    mv s5, a0
    mv s6, a1
    mv s7, a2
    li t6, 4
    beq a1, t6, ret_t1
    
    mv s1, a1
    addi s2, a2, 1
    bne s2, t6, go1
        addi s1, s1, 1
        li s2, 0
    go1:

    slli s4, a1, 2
    add s4, s4, a2
    slli s4, s4, 2
    add s4, s4, a0
    lw t1, 0(s4)
    li t5, -1
    beq t1, t5, go2
        mv a0, s5
        mv a1, s1
        mv a2, s2
        call solver
        j re
    go2:

    li s3, 0
    loop1:
        addi s3, s3, 1
        
        mv a0, s5
        mv a1, s6
        mv a2, s7
        mv a3, s3
        call is_valid
        beqz a0, go3
            sw s3, 0(s4) 
            mv a0, s5
            mv a1, s1
            mv a2, s2
            call solver
            bnez a0, ret_t1
            li t5, -1
            sw t5, 0(s4) 
        go3:
        bne s3, t6, loop1
    
    j ret_f1

ret_f1:
    li a0, 0
    j re
ret_t1:
    li a0, 1
    j re
re:
    lw s7, 28(sp)
    lw s6, 24(sp)
    lw s5, 20(sp)
    lw s4, 16(sp)
    lw s3, 12(sp)
    lw s2, 8(sp) 
    lw s1, 4(sp)
    lw ra, 0(sp)
    addi sp, sp, 32
    ret

is_valid:
    li t1, 4
    loop2: # t1 = i
        addi t1, t1, -1 
        
        slli t2, a1, 2
        add t2, t2, t1
        slli t2, t2, 2
        add t2, t2, a0
        lw t2, 0(t2)
        beq t2, a3, ret_f2
        slli t2, t1, 2
        add t2, t2, a2
        slli t2, t2, 2
        add t2, t2, a0
        lw t2, 0(t2)
        beq t2, a3, ret_f2

        bnez t1, loop2

    andi t1, a1, 0xfe # baseRow
    andi t2, a2, 0xfe # baseCol

    li t3, 4
    loop3:
        addi t3, t3, -1

        srli t4, t3, 1 # i/2
        add t4, t4, t1
        slli t4, t4, 2

        andi t5, t3, 0x01 # i%2
        add t5, t5, t2

        add t5, t5, t4

        slli t5, t5, 2
        add t5, t5, a0
        lw t5, 0(t5) 

        beq t5, a3, ret_f2
        bnez t3, loop3

    # addi t3, t1, 2
    # loop3_1:
    #     addi t3, t3, -1

    #     addi t4, t2, 2
    #     loop3_2:
    #         addi t4, t4, -1

    #         slli t5, t3, 2
    #         add t5, t5, t4
    #         slli t5, t5, 2
    #         add t5, t5, a0
    #         lw t5, 0(t5) 
    #         beq t5, a3, ret_f2 

    #         bne t4, t2, loop3_2
    #     bne t3, t1, loop3_1 
    
    j ret_t2

ret_f2:
    li a0, 0
    ret
ret_t2:
    li a0, 1
    ret

exit:
    li a0, 10 # \n
    call putchar
    li a0, 0
    ecall

fmt1:
    .string "%d"
fmt2:
    .string ", %d"