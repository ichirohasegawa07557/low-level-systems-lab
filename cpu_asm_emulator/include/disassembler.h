#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "instruction.h"

void disassemble_instruction(const Instruction *ins, unsigned index);
int disassemble_file(const char *path);

#endif
