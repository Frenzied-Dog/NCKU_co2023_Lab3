#include "emu.h"
#include "interface.h"

/* global variables */
cpu_t cpu;
bool halt = false;

/* main function */
int main(int argc, char* argv[]) {
    /* Check number of argument */
    Assert(argc == 3, "There should be 2 arguments...\n Input format should be: ./xxx.elf <entry point addr> <binary file name>");
    uint32_t entry_point_addr = strtol(argv[1], NULL, 16); // convert string to long int
    char* file_name = argv[2];

    /* initialize of the emulator */
    memset(cpu.MEM, 0, MEM_SIZE); // reset memory with zeros
    cpu.PC = entry_point_addr;    // set PC to entry point address (first instruction of the program)
    for (int i = 0; i < 32; i++) {
        cpu.GPR[i] = 0; // Register File (reset to 0)
    }
    // cpu.GPR[2] = STACK_TOP; // initialize stack pointer

    /* load test program */
    load_program(file_name);

    for (char* read_line; (read_line = rl_gets()) != NULL;) {
        char* read_line_end = read_line + strlen(read_line) + 1;
        char* cmd = strtok(read_line, " ");

        // empty input -> continue and read new line
        if (cmd == NULL) continue;

        char* args = read_line + strlen(cmd) + 1;
        if (args >= read_line_end) args = NULL;

        int i;
        for (i = 0; i < NUM_CMD; i++) {
            if (strcmp(cmd, cmd_table[i].name) == 0) {
                if (cmd_table[i].handler(args) < 0) return 0;
                break;
            }
        }
        if (i == NUM_CMD) printf("%s\n", "Unsupported command!");
    }
}
