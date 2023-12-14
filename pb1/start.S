    .section .text._start
    .global _start

_start:
    .option push
    .option norelax
    la gp, __global_pointer$
    la sp, __stack_top
    .option pop
    # go to main program
    tail prog