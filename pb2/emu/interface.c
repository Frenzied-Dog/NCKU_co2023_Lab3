#include "interface.h"

extern bool halt;
extern cpu_t cpu;

char* rl_gets(void) {
    static char* line = NULL;  // note this is static variable in a function

    if (line) {
        free(line);
        line = NULL;
    }

    line = readline(ANSI_COLOR_GREEN "(CO2023) " ANSI_COLOR_RESET);
    if (line && *line) add_history(line);

    return line;
}

int cmd_q(char* args) { return -1; }

int cmd_help(char* args) {
    if (args == NULL) {
        for (int i = 0; i < NUM_CMD; i++) {
            printf("%s: %s\n", cmd_table[i].name, cmd_table[i].description);
        }
        return 0;
    }

    int i;
    for (i = 0; i < NUM_CMD; i++) {
        if (strcmp(args, cmd_table[i].name) == 0) {
            printf("%s\n", cmd_table[i].description);
            break;
        }
    }
    if (i == NUM_CMD) {
        printf("%s\n", "help: Unknown command!");
    }
    return 0;
}

int cmd_c(char* args) {
    /* running */
    // uint64_t count=0;
    while (!halt) {
        Assert(cpu.PC < MEM_SIZE, "PC is out of memory...");
        inst_t inst_fetched = *(inst_t*)(&cpu.MEM[cpu.PC]);
        bool jumped = operation(inst_fetched);
        // count++;
        cpu.GPR[0] = 0; // $x0 should always be zero
        if (!jumped) cpu.PC += 4;    // increase PC by 4, go to next instruction
    }
    printf(ANSI_COLOR_YELLOW "End of execution, exit the emulator" ANSI_COLOR_RESET "\n");
    // printf("%lu\n", count);
    // printf("$gp:%d $a2:%d\n", cpu.GPR[3], cpu.GPR[12]);
    return 0;
}

int cmd_si(char* args) {
    if (args == NULL) {
        if (!halt) {
            Assert(cpu.PC < MEM_SIZE, "PC is out of memory...");
            inst_t inst_fetched = *(inst_t*)(&cpu.MEM[cpu.PC]);
            bool jumped = operation(inst_fetched);

            cpu.GPR[0] = 0; // $x0 should always be zero
            if (!jumped) cpu.PC += 4;    // increase PC by 4, go to next instruction
            return 0;
        }
        else {
            printf(ANSI_COLOR_YELLOW "End of execution, exit the emulator" ANSI_COLOR_RESET "\n");
            return -1;
        }
    }

    int N = atoi(strtok(args, " "));
    for (int i = 0; i < N && !halt; i++) {
        Assert(cpu.PC < MEM_SIZE, "PC is out of memory...");
        inst_t inst_fetched = *(inst_t*)(&cpu.MEM[cpu.PC]);
        bool jumped = operation(inst_fetched);

        cpu.GPR[0] = 0; // $x0 should always be zero
        if (!jumped) cpu.PC += 4;    // increase PC by 4, go to next instruction
    }

    if (halt) {
        printf(ANSI_COLOR_YELLOW "End of execution, exit the emulator" ANSI_COLOR_RESET "\n");
        return -1;
    }
    else return 0;
}

int cmd_cto(char* args) {
    if (args == NULL) {
        printf("%s\n", "info: Wrong Usage!");
        return 0;
    }

    int base_addr = strtol(strtok(NULL, " "), NULL, 16);
    while (cpu.PC != base_addr && !halt) {
        Assert(cpu.PC < MEM_SIZE, "PC is out of memory...");
        inst_t inst_fetched = *(inst_t*)(&cpu.MEM[cpu.PC]);
        bool jumped = operation(inst_fetched);

        cpu.GPR[0] = 0; // $x0 should always be zero
        if (!jumped) cpu.PC += 4;    // increase PC by 4, go to next instruction
    }

    if (halt) {
        printf(ANSI_COLOR_YELLOW "End of execution, exit the emulator" ANSI_COLOR_RESET "\n");
        return -1;
    }
    else return 0;
}

char reg_names[32][4] = { "0", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", 
                         "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
                         "t3", "t4", "t5", "t6"};

int cmd_info(char* args) {
    if (args == NULL) {
        printf("%s\n", "info: Wrong Usage!");
        return 0;
    }

    if (strcmp(args, "r") == 0) {
        printf("%s\n", "Current Register:");
        for (int i = 0; i < 32; i++) {
            if (i==0) printf("x%d (%s) : 0x%x\n", i, reg_names[i], cpu.GPR[i]);
            else if (i < 10) printf("x%d (%s): 0x%x\n", i, reg_names[i], cpu.GPR[i]);
            else if (i==26 || i==27) printf("x%d(%s):0x%x\n", i, reg_names[i], cpu.GPR[i]);
            else printf("x%d(%s): 0x%x\n", i, reg_names[i], cpu.GPR[i]);
        }
    }
    else if (strcmp(args, "pc") == 0) {
        printf("%s%#x\n", "Current PC: ", cpu.PC);
    }
    else {
        printf("%s\n", "info: Unknown command!");
    }
    return 0;
}

int cmd_x(char* args) {
    if (args == NULL) {
        printf("%s\n", "x: Wrong Usage!");
        return 0;
    }

    int N = atoi(strtok(args, " "));
    int base_addr = strtol(strtok(NULL, " "), NULL, 16);
    printf("From %#x:\n", base_addr);
    for (int i = 0; i < N; i++) {
        printf("%#010x\n", *(int32_t*)&cpu.MEM[base_addr + 4 * i]);
    }
    return 0;
}