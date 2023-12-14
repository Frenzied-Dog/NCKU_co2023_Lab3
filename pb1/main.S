    .section .text
    .global prog
    ##############
    # t1: tmp1
    # t2: tmp2
    # t3: i
    # t4: sp+4i

prog:
    tail fibo
fibo:
    addi sp, sp, -80; # sp = sp -80
    li t1, 0 
    li t2, 1 
    la a0, format
    li a1, 0
    call printf
    la a0, format
    li a1, 1
    call printf 
    li t3, 18
    sw t1, 76(sp)
    sw t2, 72(sp)     
loop:
    addi t3, t3, -1
    slli t4, t3, 2
    add t4, t4, sp; # t4 = t4 + sp
    
    lw t1, 8(t4)
    lw t2, 4(t4)
    add t1, t2, t1; # t1 = t2 + t1
    sw t1, 0(t4)
    la a0, format
    mv a1, t1 # a1 = t1 
    call printf
    bnez t3, loop # if t3 != zero then loop
    tail exit

exit:
    li a0, 0
    ecall

format:
    .string "%d\n"