#pragma once
#ifndef EMU_H
#define EMU_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "interface.h"
#include "debug.h"

/* cpu_t definition */
#define STACK_TOP 0x10000
#define MEM_SIZE 0x10000
typedef struct {
    uint32_t PC;           // Program Counter
    uint32_t GPR[32];      // General Purpose Registers
    uint8_t MEM[MEM_SIZE]; // Memory
} cpu_t;

/* instruction format definition */
typedef union {
    struct {
        uint32_t opcode : 7;
        uint32_t rd : 5;
        uint32_t func3 : 3;
        uint32_t rs1 : 5;
        uint32_t rs2 : 5;
        uint32_t func7 : 7;
    } R_TYPE;
    struct {
        uint32_t opcode : 7;
        uint32_t rd : 5;
        uint32_t func3 : 3;
        uint32_t rs1 : 5;
        int32_t imm_11_0 : 12;
    } I_TYPE;
    struct {
        uint32_t opcode : 7;
        uint32_t imm_4_0 : 5;
        uint32_t func3 : 3;
        uint32_t rs1 : 5;
        uint32_t rs2 : 5;
        int32_t imm_11_5 : 7;
    } S_TYPE;
    struct {
        uint32_t opcode : 7;
        uint32_t imm_11 : 1;
        uint32_t imm_4_1 : 4;
        uint32_t func3 : 3;
        uint32_t rs1 : 5;
        uint32_t rs2 : 5;
        uint32_t imm_10_5 : 6;
        int32_t imm_12 : 1;
    } B_TYPE;
    struct {
        uint32_t opcode : 7;
        uint32_t rd : 5;
        int32_t imm_31_12 : 20;
    } U_TYPE;
    struct {
        uint32_t opcode : 7;
        uint32_t rd : 5;
        uint32_t imm_19_12 : 8;
        uint32_t imm_11 : 1;
        uint32_t imm_10_1 : 10;
        int32_t imm_20 : 1;
    } J_TYPE;
    uint32_t raw; // raw data without decoding
} inst_t;

long fsize(FILE*);

void load_program(char*);

bool operation(inst_t);

#endif