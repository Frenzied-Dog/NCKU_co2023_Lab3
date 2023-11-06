    .section .data
case_1:
    .word -1, 1, 4, -1, -1, 3, -1, 2, -1, -1, 3, -1, -1, -1, -1, -1
    
fmt:
    .string "%d "  # The format string for printing an integer

.text
.global _start

_start:
    la a0, case_1
    jal sudoku_solver

    # Exit the program
    j exit

# Sudoku solver function
sudoku_solver:
    # Load the puzzle array address into a1
    mv a1, a0

    # Initialize variables
    li a2, 0  # i (row)
    li a3, 0  # j (column)
    li a6, 16  # Puzzle size (number of elements)
    j solve

solve:
    # Check if we have reached the end of the puzzle
    beqz a6, done

    # Load the current cell value
    lw a4, 0(a1)
    beqz a4, try_next_value

    # Cell is already filled, move to the next cell
    addi a3, a3, 1
    addi a6, a6, -1
    bnez a6, solve
    j next_row

try_next_value:
    li a4, 1
    li a5, 1
    bne a3, zero_column, next_value

zero_column:
    li a4, 1
    li a5, 0

next_value:
    sw a4, 0(a1)  # Try the current value
    addi a1, a1, 4  # Move to the next cell

    # Recursively solve the next cell
    addi a2, a2, 1
    j solve

next_row:
    li a2, 0  # Reset row
    li a3, 0  # Reset column
    j solve

done:
    # Print the solved puzzle
    la a1, case_1
    li a6, 16  # Loop counter

print_puzzle:
    lw a4, 0(a1)
    addi a1, a1, 4

    # Print the value (-1 indicates an empty cell)
    la a0, fmt   # Load format string address
    mv a1, a4   # Load the integer to print
    call printf

    addi a6, a6, -1
    bnez a6, print_puzzle

    # End the program
    j exit

exit:
    li a0, 0  # Exit syscall code
    ecall