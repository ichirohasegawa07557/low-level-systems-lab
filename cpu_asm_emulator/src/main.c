#include "assembler.h"
#include "cpu.h"
#include "disassembler.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(const char *argv0) {
    fprintf(stderr,
        "usage:\n"
        "  %s asm <input.asm> <output.bin>\n"
        "  %s run [--trace] [--max-steps N] [--dump-memory START COUNT] <program.bin>\n"
        "  %s disasm <program.bin>\n", argv0, argv0, argv0);
}

static int parse_u64(const char *s, uint64_t *out) {
    errno = 0;
    char *end = NULL;
    unsigned long long v = strtoull(s, &end, 0);
    if (errno || !end || *end != '\0') return -1;
    *out = (uint64_t)v;
    return 0;
}

static int parse_u32_arg(const char *s, uint32_t *out) {
    uint64_t v = 0;
    if (parse_u64(s, &v) != 0 || v > UINT32_MAX) return -1;
    *out = (uint32_t)v;
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) { usage(argv[0]); return 2; }
    if (strcmp(argv[1], "asm") == 0) {
        if (argc != 4) { usage(argv[0]); return 2; }
        return assemble_file(argv[2], argv[3]) == 0 ? 0 : 1;
    }
    if (strcmp(argv[1], "disasm") == 0) {
        if (argc != 3) { usage(argv[0]); return 2; }
        return disassemble_file(argv[2]) == 0 ? 0 : 1;
    }
    if (strcmp(argv[1], "run") == 0) {
        int trace = 0;
        int dump_memory = 0;
        uint32_t dump_start = 0;
        uint32_t dump_count = 0;
        uint64_t max_steps = 1000000;
        const char *path = NULL;

        for (int i = 2; i < argc; ++i) {
            if (strcmp(argv[i], "--trace") == 0) {
                trace = 1;
            } else if (strcmp(argv[i], "--max-steps") == 0) {
                if (i + 1 >= argc || parse_u64(argv[++i], &max_steps) != 0 || max_steps == 0) {
                    fprintf(stderr, "invalid --max-steps value\n");
                    return 2;
                }
            } else if (strcmp(argv[i], "--dump-memory") == 0) {
                if (i + 2 >= argc || parse_u32_arg(argv[++i], &dump_start) != 0 || parse_u32_arg(argv[++i], &dump_count) != 0) {
                    fprintf(stderr, "invalid --dump-memory arguments\n");
                    return 2;
                }
                dump_memory = 1;
            } else if (argv[i][0] == '-') {
                usage(argv[0]);
                return 2;
            } else {
                if (path != NULL) { usage(argv[0]); return 2; }
                path = argv[i];
            }
        }
        if (!path) { usage(argv[0]); return 2; }

        Instruction program[VM_MAX_PROGRAM]; uint32_t count = 0;
        if (cpu_load_program(path, program, &count) != 0) return 1;
        CPU cpu; cpu_init(&cpu); cpu.trace = trace; cpu.max_steps = max_steps;
        int rc = cpu_run(&cpu, program, count);
        cpu_dump(&cpu);
        if (dump_memory) cpu_dump_memory(&cpu, dump_start, dump_count);
        return rc == 0 ? 0 : 1;
    }
    usage(argv[0]); return 2;
}
