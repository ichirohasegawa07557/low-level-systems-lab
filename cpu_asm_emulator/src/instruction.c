#include "instruction.h"
#include <ctype.h>
#include <string.h>

static int ieq(const char *a, const char *b) {
    while (*a && *b) {
        if (toupper((unsigned char)*a) != toupper((unsigned char)*b)) return 0;
        ++a;
        ++b;
    }
    return *a == '\0' && *b == '\0';
}

const char *opcode_name(OpCode op) {
    switch (op) {
        case OP_NOP: return "NOP";
        case OP_MOV_RI: return "MOV_RI";
        case OP_MOV_RR: return "MOV_RR";
        case OP_ADD: return "ADD";
        case OP_SUB: return "SUB";
        case OP_MUL: return "MUL";
        case OP_DIV: return "DIV";
        case OP_MOD: return "MOD";
        case OP_AND: return "AND";
        case OP_OR: return "OR";
        case OP_XOR: return "XOR";
        case OP_NOT: return "NOT";
        case OP_SHL: return "SHL";
        case OP_SHR: return "SHR";
        case OP_LOAD_RA: return "LOAD_RA";
        case OP_LOAD_RR: return "LOAD_RR";
        case OP_STORE_RA: return "STORE_RA";
        case OP_STORE_RR: return "STORE_RR";
        case OP_CMP_RR: return "CMP_RR";
        case OP_CMP_RI: return "CMP_RI";
        case OP_JMP: return "JMP";
        case OP_JE: return "JE";
        case OP_JNE: return "JNE";
        case OP_JG: return "JG";
        case OP_JL: return "JL";
        case OP_JGE: return "JGE";
        case OP_JLE: return "JLE";
        case OP_PUSH: return "PUSH";
        case OP_POP: return "POP";
        case OP_CALL: return "CALL";
        case OP_RET: return "RET";
        case OP_PRINT: return "PRINT";
        case OP_PRINTM: return "PRINTM";
        case OP_HALT: return "HALT";
        default: return "UNKNOWN";
    }
}

int opcode_from_name(const char *name, OpCode *out) {
    if (ieq(name, "NOP")) { *out = OP_NOP; return 0; }
    if (ieq(name, "ADD")) { *out = OP_ADD; return 0; }
    if (ieq(name, "SUB")) { *out = OP_SUB; return 0; }
    if (ieq(name, "MUL")) { *out = OP_MUL; return 0; }
    if (ieq(name, "DIV")) { *out = OP_DIV; return 0; }
    if (ieq(name, "MOD")) { *out = OP_MOD; return 0; }
    if (ieq(name, "AND")) { *out = OP_AND; return 0; }
    if (ieq(name, "OR")) { *out = OP_OR; return 0; }
    if (ieq(name, "XOR")) { *out = OP_XOR; return 0; }
    if (ieq(name, "NOT")) { *out = OP_NOT; return 0; }
    if (ieq(name, "SHL")) { *out = OP_SHL; return 0; }
    if (ieq(name, "SHR")) { *out = OP_SHR; return 0; }
    if (ieq(name, "JMP")) { *out = OP_JMP; return 0; }
    if (ieq(name, "JE")) { *out = OP_JE; return 0; }
    if (ieq(name, "JNE")) { *out = OP_JNE; return 0; }
    if (ieq(name, "JG")) { *out = OP_JG; return 0; }
    if (ieq(name, "JL")) { *out = OP_JL; return 0; }
    if (ieq(name, "JGE")) { *out = OP_JGE; return 0; }
    if (ieq(name, "JLE")) { *out = OP_JLE; return 0; }
    if (ieq(name, "PUSH")) { *out = OP_PUSH; return 0; }
    if (ieq(name, "POP")) { *out = OP_POP; return 0; }
    if (ieq(name, "CALL")) { *out = OP_CALL; return 0; }
    if (ieq(name, "RET")) { *out = OP_RET; return 0; }
    if (ieq(name, "PRINT")) { *out = OP_PRINT; return 0; }
    if (ieq(name, "PRINTM")) { *out = OP_PRINTM; return 0; }
    if (ieq(name, "HALT")) { *out = OP_HALT; return 0; }
    if (ieq(name, "MOV")) { *out = OP_MOV_RI; return 0; }
    if (ieq(name, "LOAD")) { *out = OP_LOAD_RA; return 0; }
    if (ieq(name, "STORE")) { *out = OP_STORE_RA; return 0; }
    if (ieq(name, "CMP")) { *out = OP_CMP_RR; return 0; }
    return -1;
}

int opcode_is_branch(OpCode op) {
    return op == OP_JMP || op == OP_JE || op == OP_JNE || op == OP_JG || op == OP_JL || op == OP_JGE || op == OP_JLE || op == OP_CALL;
}
