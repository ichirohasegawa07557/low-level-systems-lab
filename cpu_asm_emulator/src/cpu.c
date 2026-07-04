#include "cpu.h"
#include "disassembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cpu_init(CPU *cpu) {
    memset(cpu, 0, sizeof(*cpu));
    cpu->max_steps = 1000000;
}

int cpu_load_program(const char *path, Instruction *program, uint32_t *count) {
    FILE *fp = fopen(path, "rb");
    if (!fp) { perror(path); return -1; }
    ProgramHeader header;
    if (fread(&header, sizeof(header), 1, fp) != 1) { fprintf(stderr, "invalid bytecode header\n"); fclose(fp); return -1; }
    if (header.magic != VM_MAGIC || header.version != VM_VERSION || header.count > VM_MAX_PROGRAM) {
        fprintf(stderr, "unsupported bytecode file: magic=%08x version=%u count=%u\n", header.magic, header.version, header.count);
        fclose(fp); return -1;
    }
    if (fread(program, sizeof(Instruction), header.count, fp) != header.count) {
        fprintf(stderr, "truncated bytecode file\n"); fclose(fp); return -1;
    }
    fclose(fp);
    *count = header.count;
    return 0;
}

static int addr_ok(int32_t addr) { return addr >= 0 && addr < VM_MEMORY_WORDS; }

static int jump_to(CPU *cpu, int32_t target, uint32_t count) {
    if (target < 0 || (uint32_t)target >= count) {
        fprintf(stderr, "invalid jump target %d\n", target); return -1;
    }
    cpu->pc = (uint32_t)target;
    return 0;
}

int cpu_run(CPU *cpu, const Instruction *program, uint32_t count) {
    while (!cpu->halted) {
        if (cpu->pc >= count) { fprintf(stderr, "pc out of range: %u\n", cpu->pc); return -1; }
        if (cpu->steps++ >= cpu->max_steps) { fprintf(stderr, "max steps reached; possible infinite loop\n"); return -1; }
        const Instruction *ins = &program[cpu->pc];
        if (cpu->trace) {
            printf("pc=%04u ", cpu->pc);
            disassemble_instruction(ins, cpu->pc);
        }
        cpu->pc++;
        switch ((OpCode)ins->op) {
            case OP_NOP: break;
            case OP_MOV_RI: cpu->regs[ins->a] = ins->imm; break;
            case OP_MOV_RR: cpu->regs[ins->a] = cpu->regs[ins->b]; break;
            case OP_ADD: cpu->regs[ins->a] = cpu->regs[ins->b] + cpu->regs[ins->c]; break;
            case OP_SUB: cpu->regs[ins->a] = cpu->regs[ins->b] - cpu->regs[ins->c]; break;
            case OP_MUL: cpu->regs[ins->a] = cpu->regs[ins->b] * cpu->regs[ins->c]; break;
            case OP_DIV:
                if (cpu->regs[ins->c] == 0) { fprintf(stderr, "division by zero\n"); return -1; }
                cpu->regs[ins->a] = cpu->regs[ins->b] / cpu->regs[ins->c]; break;
            case OP_LOAD_RA:
                if (!addr_ok(ins->imm)) { fprintf(stderr, "LOAD invalid address %d\n", ins->imm); return -1; }
                cpu->regs[ins->a] = cpu->memory[ins->imm]; break;
            case OP_LOAD_RR: {
                int32_t addr = cpu->regs[ins->b];
                if (!addr_ok(addr)) { fprintf(stderr, "LOAD invalid address in R%d: %d\n", ins->b, addr); return -1; }
                cpu->regs[ins->a] = cpu->memory[addr]; break;
            }
            case OP_STORE_RA:
                if (!addr_ok(ins->imm)) { fprintf(stderr, "STORE invalid address %d\n", ins->imm); return -1; }
                cpu->memory[ins->imm] = cpu->regs[ins->a]; break;
            case OP_STORE_RR: {
                int32_t addr = cpu->regs[ins->b];
                if (!addr_ok(addr)) { fprintf(stderr, "STORE invalid address in R%d: %d\n", ins->b, addr); return -1; }
                cpu->memory[addr] = cpu->regs[ins->a]; break;
            }
            case OP_CMP_RR: {
                int32_t l = cpu->regs[ins->a], r = cpu->regs[ins->b];
                cpu->cmp = (l > r) - (l < r); break;
            }
            case OP_CMP_RI: {
                int32_t l = cpu->regs[ins->a], r = ins->imm;
                cpu->cmp = (l > r) - (l < r); break;
            }
            case OP_JMP: if (jump_to(cpu, ins->imm, count) != 0) return -1; break;
            case OP_JE: if (cpu->cmp == 0 && jump_to(cpu, ins->imm, count) != 0) return -1; break;
            case OP_JNE: if (cpu->cmp != 0 && jump_to(cpu, ins->imm, count) != 0) return -1; break;
            case OP_JG: if (cpu->cmp > 0 && jump_to(cpu, ins->imm, count) != 0) return -1; break;
            case OP_JL: if (cpu->cmp < 0 && jump_to(cpu, ins->imm, count) != 0) return -1; break;
            case OP_PRINT: printf("R%d = %d\n", ins->a, cpu->regs[ins->a]); break;
            case OP_HALT: cpu->halted = true; break;
            default: fprintf(stderr, "unknown opcode %d\n", ins->op); return -1;
        }
    }
    return 0;
}

void cpu_dump(const CPU *cpu) {
    printf("registers:");
    for (int i = 0; i < VM_MAX_REGS; ++i) printf(" R%d=%d", i, cpu->regs[i]);
    printf(" pc=%u cmp=%d steps=%llu\n", cpu->pc, cpu->cmp, (unsigned long long)cpu->steps);
}


void cpu_dump_memory(const CPU *cpu, uint32_t start, uint32_t count) {
    if (start >= VM_MEMORY_WORDS) {
        printf("memory dump: start address out of range\n");
        return;
    }
    uint32_t end = start + count;
    if (end > VM_MEMORY_WORDS || end < start) end = VM_MEMORY_WORDS;
    printf("memory[%u..%u):", start, end);
    for (uint32_t i = start; i < end; ++i) {
        printf(" [%u]=%d", i, cpu->memory[i]);
    }
    printf("\n");
}
