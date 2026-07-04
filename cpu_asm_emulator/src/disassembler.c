#include "disassembler.h"
#include "cpu.h"
#include <stdio.h>

void disassemble_instruction(const Instruction *ins, unsigned index) {
    (void)index;
    switch ((OpCode)ins->op) {
        case OP_NOP: printf("NOP\n"); break;
        case OP_MOV_RI: printf("MOV R%d, %d\n", ins->a, ins->imm); break;
        case OP_MOV_RR: printf("MOV R%d, R%d\n", ins->a, ins->b); break;
        case OP_ADD: printf("ADD R%d, R%d, R%d\n", ins->a, ins->b, ins->c); break;
        case OP_SUB: printf("SUB R%d, R%d, R%d\n", ins->a, ins->b, ins->c); break;
        case OP_MUL: printf("MUL R%d, R%d, R%d\n", ins->a, ins->b, ins->c); break;
        case OP_DIV: printf("DIV R%d, R%d, R%d\n", ins->a, ins->b, ins->c); break;
        case OP_LOAD_RA: printf("LOAD R%d, [%d]\n", ins->a, ins->imm); break;
        case OP_LOAD_RR: printf("LOAD R%d, [R%d]\n", ins->a, ins->b); break;
        case OP_STORE_RA: printf("STORE R%d, [%d]\n", ins->a, ins->imm); break;
        case OP_STORE_RR: printf("STORE R%d, [R%d]\n", ins->a, ins->b); break;
        case OP_CMP_RR: printf("CMP R%d, R%d\n", ins->a, ins->b); break;
        case OP_CMP_RI: printf("CMP R%d, %d\n", ins->a, ins->imm); break;
        case OP_JMP: printf("JMP %d\n", ins->imm); break;
        case OP_JE: printf("JE %d\n", ins->imm); break;
        case OP_JNE: printf("JNE %d\n", ins->imm); break;
        case OP_JG: printf("JG %d\n", ins->imm); break;
        case OP_JL: printf("JL %d\n", ins->imm); break;
        case OP_PRINT: printf("PRINT R%d\n", ins->a); break;
        case OP_HALT: printf("HALT\n"); break;
        default: printf("DATA op=%d a=%d b=%d c=%d imm=%d\n", ins->op, ins->a, ins->b, ins->c, ins->imm); break;
    }
}

int disassemble_file(const char *path) {
    Instruction program[VM_MAX_PROGRAM];
    uint32_t count = 0;
    if (cpu_load_program(path, program, &count) != 0) return -1;
    for (uint32_t i = 0; i < count; ++i) {
        printf("%04u: ", i);
        disassemble_instruction(&program[i], i);
    }
    return 0;
}
