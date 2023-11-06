#include "emu.h"

extern cpu_t cpu;
extern bool halt;

/* util functions */
long fsize(FILE* fp) {
    long now = ftell(fp);
    Perror(now != -1, "Fail to return the file position");
    int ret = fseek(fp, 0, SEEK_END);
    Perror(ret != -1, "Fail to seek the end of the file");
    long size = ftell(fp);
    Perror(now != -1, "Fail to return the file position");
    ret = fseek(fp, now, SEEK_SET);
    Perror(ret != -1, "Fail to seek the beginning of the file");
    return size;
}

void load_program(char* file_name) {
    FILE* fp = fopen(file_name, "r");
    Perror(fp != NULL, "Fail to open the file");
    long size = fsize(fp);
    Assert(size <= MEM_SIZE, "The size of the test program is larger than CPU memory");
    long read = fread(cpu.MEM, 1, size, fp); // load data from file into memory
    Assert(size == read, "Fail to read all contents of the test program");
    fclose(fp);
}

bool operation(inst_t inst_fetched) {
    // uint32_t ra = cpu.GPR[1];
    // uint32_t sp = cpu.GPR[2];
    // uint32_t nRow_s1 = cpu.GPR[9];
    // uint32_t nCol_s2 = cpu.GPR[18];
    // uint32_t row_a1 = cpu.GPR[11];
    // uint32_t col_a2 = cpu.GPR[12];
    // uint32_t num_a3 = cpu.GPR[13];
    // int32_t t1 = cpu.GPR[6];
    // int32_t t2 = cpu.GPR[7];
    
    uint32_t opcode = inst_fetched.raw & 0b1111111;
    int32_t offset;
    uint32_t tmp;
    switch (opcode) {
    case 0b0110111: // LUI
        cpu.GPR[inst_fetched.U_TYPE.rd] = (inst_fetched.U_TYPE.imm_31_12 << 12);
        break;
    case 0b0010111: // AUIPC
        cpu.GPR[inst_fetched.U_TYPE.rd] = cpu.PC + (inst_fetched.U_TYPE.imm_31_12 << 12);
        break;
    case 0b1101111: // JAL
        cpu.GPR[inst_fetched.J_TYPE.rd] = cpu.PC + 4;
        cpu.PC += (inst_fetched.J_TYPE.imm_20 << 20) + (inst_fetched.J_TYPE.imm_19_12 << 12) + (inst_fetched.J_TYPE.imm_11 << 11) + (inst_fetched.J_TYPE.imm_10_1 << 1);
        return true;
    case 0b1100111: // JALR
        tmp = cpu.PC + 4;
        cpu.PC = (cpu.GPR[inst_fetched.I_TYPE.rs1] + inst_fetched.I_TYPE.imm_11_0) & 0xFFFFFE;
        cpu.GPR[inst_fetched.I_TYPE.rd] = tmp;
        return true;
    case 0b1100011: // B-Type inst
        offset = (inst_fetched.B_TYPE.imm_12 << 12) + (inst_fetched.B_TYPE.imm_11 << 11) + (inst_fetched.B_TYPE.imm_10_5 << 5) + (inst_fetched.B_TYPE.imm_4_1 << 1);
        switch (inst_fetched.B_TYPE.func3) {
        case 0b000: // BEQ
            if (cpu.GPR[inst_fetched.B_TYPE.rs1] == cpu.GPR[inst_fetched.B_TYPE.rs2]) {
                cpu.PC += offset;
                return true;
            }
            break;
        case 0b001: // BNE
            if (cpu.GPR[inst_fetched.B_TYPE.rs1] != cpu.GPR[inst_fetched.B_TYPE.rs2]) {
                cpu.PC += offset;
                return true;
            }
            break;
        case 0b100: // BLT
            if ((int32_t)cpu.GPR[inst_fetched.B_TYPE.rs1] < (int32_t)cpu.GPR[inst_fetched.B_TYPE.rs2]) {
                cpu.PC += offset;
                return true;
            }
            break;
        case 0b101: // BGE
            if ((int32_t)cpu.GPR[inst_fetched.B_TYPE.rs1] >= (int32_t)cpu.GPR[inst_fetched.B_TYPE.rs2]) {
                cpu.PC += offset;
                return true;
            }
            break;
        case 0b110: // BLTU
            if (cpu.GPR[inst_fetched.B_TYPE.rs1] < cpu.GPR[inst_fetched.B_TYPE.rs2]) {
                cpu.PC += offset;
                return true;
            }
            break;
        case 0b111: // BGEU
            if (cpu.GPR[inst_fetched.B_TYPE.rs1] >= cpu.GPR[inst_fetched.B_TYPE.rs2]) {
                cpu.PC += offset;
                return true;
            }
            break;
        default:
            Panic("Unsupported instruction 0x%08x in 0x%x", inst_fetched.raw, cpu.PC);
            break;
        }
        break;
    case 0b0000011: // I-Type
        switch (inst_fetched.I_TYPE.func3) {
        case 0b000: // LB
            cpu.GPR[inst_fetched.I_TYPE.rd] = (int8_t)cpu.MEM[cpu.GPR[inst_fetched.I_TYPE.rs1] + inst_fetched.I_TYPE.imm_11_0];
            break;
        case 0b001: // LH
            cpu.GPR[inst_fetched.I_TYPE.rd] = *(int16_t*)&cpu.MEM[cpu.GPR[inst_fetched.I_TYPE.rs1] + inst_fetched.I_TYPE.imm_11_0];
            break;
        case 0b010: // LW
            cpu.GPR[inst_fetched.I_TYPE.rd] = *(int32_t*)&cpu.MEM[cpu.GPR[inst_fetched.I_TYPE.rs1] + inst_fetched.I_TYPE.imm_11_0];
            break;
        case 0b100: // LBU
            cpu.GPR[inst_fetched.I_TYPE.rd] = cpu.MEM[cpu.GPR[inst_fetched.I_TYPE.rs1] + inst_fetched.I_TYPE.imm_11_0];
            break;
        case 0b101: // LHU
            cpu.GPR[inst_fetched.I_TYPE.rd] = *(uint16_t*)&cpu.MEM[cpu.GPR[inst_fetched.I_TYPE.rs1] + inst_fetched.I_TYPE.imm_11_0];
            break;
        default:
            Panic("Unsupported instruction 0x%08x in 0x%x", inst_fetched.raw, cpu.PC);
            break;
        }
        break;
    case 0b0100011: // S-Type inst
        offset = (inst_fetched.S_TYPE.imm_11_5 << 5) + inst_fetched.S_TYPE.imm_4_0;
        switch (inst_fetched.S_TYPE.func3) {
        case 0b000: // SB
            cpu.MEM[cpu.GPR[inst_fetched.S_TYPE.rs1] + offset] = cpu.GPR[inst_fetched.S_TYPE.rs2];
            break;
        case 0b001: // SH
            *(uint16_t*)&cpu.MEM[cpu.GPR[inst_fetched.S_TYPE.rs1] + offset] = cpu.GPR[inst_fetched.S_TYPE.rs2];
            break;
        case 0b010: // SW
            *(uint32_t*)&cpu.MEM[cpu.GPR[inst_fetched.S_TYPE.rs1] + offset] = cpu.GPR[inst_fetched.S_TYPE.rs2];
            break;
        default:
            Panic("Unsupported instruction 0x%08x in 0x%x", inst_fetched.raw, cpu.PC);
            break;
        }
        break;
    case 0b0010011: // I-Type inst
        switch (inst_fetched.I_TYPE.func3) {
        case 0b000: // ADDI
            cpu.GPR[inst_fetched.I_TYPE.rd] = cpu.GPR[inst_fetched.I_TYPE.rs1] + inst_fetched.I_TYPE.imm_11_0;
            break;
        case 0b010: // SLTI
            cpu.GPR[inst_fetched.I_TYPE.rd] = (int32_t)(cpu.GPR[inst_fetched.I_TYPE.rs1]) < inst_fetched.I_TYPE.imm_11_0;
            break;
        case 0b011: // SLTIU
            cpu.GPR[inst_fetched.I_TYPE.rd] = cpu.GPR[inst_fetched.I_TYPE.rs1] < (uint32_t)inst_fetched.I_TYPE.imm_11_0;
            break;
        case 0b100: // XORI
            cpu.GPR[inst_fetched.I_TYPE.rd] = cpu.GPR[inst_fetched.I_TYPE.rs1] ^ inst_fetched.I_TYPE.imm_11_0;
            break;
        case 0b110: // ORI
            cpu.GPR[inst_fetched.I_TYPE.rd] = cpu.GPR[inst_fetched.I_TYPE.rs1] | inst_fetched.I_TYPE.imm_11_0;
            break;
        case 0b111: // ANDI
            cpu.GPR[inst_fetched.I_TYPE.rd] = cpu.GPR[inst_fetched.I_TYPE.rs1] & inst_fetched.I_TYPE.imm_11_0;
            break;
        case 0b001: // SLLI
            cpu.GPR[inst_fetched.I_TYPE.rd] = cpu.GPR[inst_fetched.I_TYPE.rs1] << inst_fetched.I_TYPE.imm_11_0;
            break;
        case 0b101:
            if (inst_fetched.I_TYPE.imm_11_0 >> 5 == 0b0) { // SRLI 
                cpu.GPR[inst_fetched.I_TYPE.rd] = cpu.GPR[inst_fetched.I_TYPE.rs1] >> inst_fetched.I_TYPE.imm_11_0;
            }
            else if (inst_fetched.I_TYPE.imm_11_0 >> 5 == 0b0100000) { // SRAI
                offset = inst_fetched.I_TYPE.imm_11_0 & 0b11111;
                cpu.GPR[inst_fetched.I_TYPE.rd] = (int32_t)cpu.GPR[inst_fetched.I_TYPE.rs1] >> offset;
            }
            else Panic("Unsupported instruction 0x%08x in 0x%x", inst_fetched.raw, cpu.PC);
            break;
        default:
            Panic("Unsupported instruction 0x%08x in 0x%x", inst_fetched.raw, cpu.PC);
            break;
        }
        break;
    case 0b0110011: // R-Type inst
        switch (inst_fetched.R_TYPE.func3) {
        case 0b000:
            if (inst_fetched.R_TYPE.func7 == 0b0) { // ADD
                cpu.GPR[inst_fetched.R_TYPE.rd] = cpu.GPR[inst_fetched.R_TYPE.rs1] + cpu.GPR[inst_fetched.R_TYPE.rs2];
            }
            else if (inst_fetched.R_TYPE.func7 == 0b0100000) { // SUB
                cpu.GPR[inst_fetched.R_TYPE.rd] = cpu.GPR[inst_fetched.R_TYPE.rs1] - cpu.GPR[inst_fetched.R_TYPE.rs2];
            }
            else Panic("Unsupported instruction 0x%08x in 0x%x", inst_fetched.raw, cpu.PC);
            break;
        case 0b001: // SLL
            cpu.GPR[inst_fetched.R_TYPE.rd] = cpu.GPR[inst_fetched.R_TYPE.rs1] << (cpu.GPR[inst_fetched.R_TYPE.rs2] & 0x1f);
            break;
        case 0b010: // SLT
            cpu.GPR[inst_fetched.R_TYPE.rd] = (int32_t)cpu.GPR[inst_fetched.R_TYPE.rs1] < (int32_t)cpu.GPR[inst_fetched.R_TYPE.rs2];
            break;
        case 0b011: // SLTU
            cpu.GPR[inst_fetched.R_TYPE.rd] = cpu.GPR[inst_fetched.R_TYPE.rs1] < cpu.GPR[inst_fetched.R_TYPE.rs2];
            break;
        case 0b100: // XOR
            cpu.GPR[inst_fetched.R_TYPE.rd] = cpu.GPR[inst_fetched.R_TYPE.rs1] ^ cpu.GPR[inst_fetched.R_TYPE.rs2];
            break;
        case 0b101:
            offset = cpu.GPR[inst_fetched.R_TYPE.rs2] & 0x1f;
            if (inst_fetched.R_TYPE.func7 == 0b0) { // SRL
                cpu.GPR[inst_fetched.R_TYPE.rd] = cpu.GPR[inst_fetched.R_TYPE.rs1] >> offset;
            }
            else if (inst_fetched.R_TYPE.func7 == 0b0100000) { // SRA
                cpu.GPR[inst_fetched.R_TYPE.rd] = (int32_t)cpu.GPR[inst_fetched.R_TYPE.rs1] >> offset;
            }
            else Panic("Unsupported instruction 0x%08x in 0x%x", inst_fetched.raw, cpu.PC);
            break;
        case 0b110: // OR
            cpu.GPR[inst_fetched.R_TYPE.rd] = cpu.GPR[inst_fetched.R_TYPE.rs1] | cpu.GPR[inst_fetched.R_TYPE.rs2];
            break;
        case 0b111: // AND
            cpu.GPR[inst_fetched.R_TYPE.rd] = cpu.GPR[inst_fetched.R_TYPE.rs1] & cpu.GPR[inst_fetched.R_TYPE.rs2];
            break;
        default:
            break;
        }
        break;
    case 0b1110011:
        // ECALL or EBREAK
        if (inst_fetched.I_TYPE.imm_11_0 == 0) {
            // ECALL
            if (cpu.GPR[10] == 0) {
                /* stop execution, set up halt signal */
                halt = true;
            }
            else if (cpu.GPR[10] == 1) {
                /* put single char to stdout */
                putc(cpu.GPR[11], stdout);
            }
            else {
                Panic("Unsupported service request type, value in $a0 is %d", cpu.GPR[10]);
            }
        }
        else {
            Panic("Unsupported instruction 0x%08x in 0x%x", inst_fetched.raw, cpu.PC);
        }
        break;
    default:
        Panic("Unsupported instruction 0x%08x in 0x%x", inst_fetched.raw, cpu.PC);
        break;
    }
    return false;
}
