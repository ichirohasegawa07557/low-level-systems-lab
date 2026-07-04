#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>
#include <stddef.h>

#define VM_MAGIC 0x4550434Du
#define VM_VERSION 2u
#define VM_MAX_REGS 8
#define VM_MEMORY_WORDS 4096
#define VM_MAX_PROGRAM 8192
#define VM_MAX_BREAKPOINTS 64

typedef enum {
    OP_NOP = 0,
    OP_MOV_RI,
    OP_MOV_RR,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_AND,
    OP_OR,
    OP_XOR,
    OP_NOT,
    OP_SHL,
    OP_SHR,
    OP_LOAD_RA,
    OP_LOAD_RR,
    OP_STORE_RA,
    OP_STORE_RR,
    OP_CMP_RR,
    OP_CMP_RI,
    OP_JMP,
    OP_JE,
    OP_JNE,
    OP_JG,
    OP_JL,
    OP_JGE,
    OP_JLE,
    OP_PUSH,
    OP_POP,
    OP_CALL,
    OP_RET,
    OP_PRINT,
    OP_PRINTM,
    OP_HALT
} OpCode;

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t count;
} ProgramHeader;

typedef struct {
    int32_t op;
    int32_t a;
    int32_t b;
    int32_t c;
    int32_t imm;
} Instruction;

const char *opcode_name(OpCode op);
int opcode_from_name(const char *name, OpCode *out);
int opcode_is_branch(OpCode op);

#endif
