#ifndef CPU_H
#define CPU_H

#include "instruction.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int32_t regs[VM_MAX_REGS];
    int32_t memory[VM_MEMORY_WORDS];
    int32_t cmp;
    uint32_t pc;
    uint32_t sp;
    bool halted;
    bool trace;
    bool debug_stop;
    uint64_t steps;
    uint64_t max_steps;
    uint32_t breakpoints[VM_MAX_BREAKPOINTS];
    uint32_t breakpoint_count;
    bool watch_enabled;
    uint32_t watch_start;
    uint32_t watch_count;
} CPU;

void cpu_init(CPU *cpu);
int cpu_load_program(const char *path, Instruction *program, uint32_t *count);
int cpu_run(CPU *cpu, const Instruction *program, uint32_t count);
int cpu_add_breakpoint(CPU *cpu, uint32_t pc);
void cpu_set_watch(CPU *cpu, uint32_t start, uint32_t count);
void cpu_dump(const CPU *cpu);
void cpu_dump_memory(const CPU *cpu, uint32_t start, uint32_t count);

#endif
