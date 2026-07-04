#include "assembler.h"
#include "instruction.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 8192
#define MAX_LABELS 1024
#define LINE_LEN 256
#define TOKEN_LEN 64

typedef struct { char name[TOKEN_LEN]; int address; } Label;

typedef struct {
    char text[LINE_LEN];
    int source_line;
} SourceLine;

typedef struct {
    Label labels[MAX_LABELS];
    int label_count;
} SymbolTable;

static void strip_comment(char *s) {
    for (char *p = s; *p; ++p) {
        if (*p == ';' || *p == '#') { *p = '\0'; return; }
    }
}

static char *trim(char *s) {
    while (isspace((unsigned char)*s)) s++;
    if (*s == '\0') return s;
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) *end-- = '\0';
    return s;
}

static void normalize_separators(char *s) {
    for (char *p = s; *p; ++p) if (*p == ',') *p = ' ';
}

static int parse_reg(const char *tok) {
    if (!tok || (tok[0] != 'R' && tok[0] != 'r')) return -1;
    char *end = NULL;
    long v = strtol(tok + 1, &end, 10);
    if (!end || *end != '\0' || v < 0 || v >= VM_MAX_REGS) return -1;
    return (int)v;
}

static int parse_i32(const char *tok, int32_t *out) {
    if (!tok) return -1;
    errno = 0;
    char *end = NULL;
    long v = strtol(tok, &end, 0);
    if (errno || !end || *end != '\0' || v < INT32_MIN || v > INT32_MAX) return -1;
    *out = (int32_t)v;
    return 0;
}

static int parse_mem_operand(const char *tok, int *is_reg, int32_t *value) {
    if (!tok || tok[0] != '[') return -1;
    size_t n = strlen(tok);
    if (n < 3 || tok[n - 1] != ']') return -1;
    char inner[TOKEN_LEN];
    if (n - 2 >= sizeof(inner)) return -1;
    memcpy(inner, tok + 1, n - 2);
    inner[n - 2] = '\0';
    int r = parse_reg(inner);
    if (r >= 0) { *is_reg = 1; *value = r; return 0; }
    int32_t addr = 0;
    if (parse_i32(inner, &addr) == 0) { *is_reg = 0; *value = addr; return 0; }
    return -1;
}

static int add_label(SymbolTable *st, const char *name, int address, int line) {
    if ((int)strlen(name) <= 0 || strlen(name) >= TOKEN_LEN) {
        fprintf(stderr, "line %d: invalid label name\n", line); return -1;
    }
    for (int i = 0; i < st->label_count; ++i) {
        if (strcmp(st->labels[i].name, name) == 0) {
            fprintf(stderr, "line %d: duplicate label '%s'\n", line, name); return -1;
        }
    }
    if (st->label_count >= MAX_LABELS) { fprintf(stderr, "too many labels\n"); return -1; }
    snprintf(st->labels[st->label_count].name, TOKEN_LEN, "%s", name);
    st->labels[st->label_count].address = address;
    st->label_count++;
    return 0;
}

static int find_label(const SymbolTable *st, const char *name, int32_t *out) {
    for (int i = 0; i < st->label_count; ++i) {
        if (strcmp(st->labels[i].name, name) == 0) {
            *out = st->labels[i].address;
            return 0;
        }
    }
    return -1;
}

static int collect_lines(const char *path, SourceLine *lines, int *line_count) {
    FILE *fp = fopen(path, "r");
    if (!fp) { perror(path); return -1; }
    char buf[LINE_LEN];
    int n = 0;
    int source_line = 1;
    while (fgets(buf, sizeof(buf), fp)) {
        if (n >= MAX_LINES) { fprintf(stderr, "too many source lines\n"); fclose(fp); return -1; }
        strip_comment(buf);
        char *t = trim(buf);
        if (*t) {
            snprintf(lines[n].text, sizeof(lines[n].text), "%s", t);
            lines[n].source_line = source_line;
            n++;
        }
        source_line++;
    }
    fclose(fp);
    *line_count = n;
    return 0;
}

static int first_pass(SourceLine *lines, int line_count, SymbolTable *st) {
    int pc = 0;
    for (int i = 0; i < line_count; ++i) {
        char tmp[LINE_LEN]; strncpy(tmp, lines[i].text, sizeof(tmp) - 1); tmp[sizeof(tmp) - 1] = '\0';
        char *s = trim(tmp);
        char *colon = strchr(s, ':');
        if (colon) {
            *colon = '\0';
            char *name = trim(s);
            if (add_label(st, name, pc, lines[i].source_line) != 0) return -1;
            s = trim(colon + 1);
            if (*s == '\0') continue;
        }
        pc++;
    }
    return 0;
}

static int tokenize(char *line, char **tok, int max_tok) {
    normalize_separators(line);
    int n = 0;
    char *save = NULL;
    for (char *p = strtok_r(line, " \t\r\n", &save); p && n < max_tok; p = strtok_r(NULL, " \t\r\n", &save)) {
        tok[n++] = p;
    }
    return n;
}

static int encode_line(char *line, int source_line, const SymbolTable *st, Instruction *out) {
    memset(out, 0, sizeof(*out));
    char *s = trim(line);
    char *colon = strchr(s, ':');
    if (colon) {
        s = trim(colon + 1);
        if (*s == '\0') return 1; /* label-only */
    }
    char work[LINE_LEN]; snprintf(work, sizeof(work), "%s", s);
    char *tok[5] = {0};
    int ntok = tokenize(work, tok, 5);
    if (ntok == 0) return 1;

    OpCode base;
    if (opcode_from_name(tok[0], &base) != 0) {
        fprintf(stderr, "line %d: unknown opcode '%s'\n", source_line, tok[0]); return -1;
    }

    if (base == OP_NOP || base == OP_HALT) {
        if (ntok != 1) { fprintf(stderr, "line %d: %s takes no operands\n", source_line, tok[0]); return -1; }
        out->op = base; return 0;
    }

    if (base == OP_PRINT) {
        if (ntok != 2) { fprintf(stderr, "line %d: PRINT Rn\n", source_line); return -1; }
        int r = parse_reg(tok[1]);
        if (r < 0) { fprintf(stderr, "line %d: invalid register '%s'\n", source_line, tok[1]); return -1; }
        out->op = OP_PRINT; out->a = r; return 0;
    }

    if (base == OP_MOV_RI) {
        if (ntok != 3) { fprintf(stderr, "line %d: MOV Rdst, Rsrc|imm\n", source_line); return -1; }
        int rd = parse_reg(tok[1]);
        int rs = parse_reg(tok[2]);
        if (rd < 0) { fprintf(stderr, "line %d: invalid destination register\n", source_line); return -1; }
        if (rs >= 0) { out->op = OP_MOV_RR; out->a = rd; out->b = rs; return 0; }
        int32_t imm = 0;
        if (parse_i32(tok[2], &imm) != 0) { fprintf(stderr, "line %d: invalid immediate '%s'\n", source_line, tok[2]); return -1; }
        out->op = OP_MOV_RI; out->a = rd; out->imm = imm; return 0;
    }

    if (base == OP_ADD || base == OP_SUB || base == OP_MUL || base == OP_DIV) {
        if (ntok != 4) { fprintf(stderr, "line %d: %s Rdst, Rsrc1, Rsrc2\n", source_line, tok[0]); return -1; }
        int rd = parse_reg(tok[1]), r1 = parse_reg(tok[2]), r2 = parse_reg(tok[3]);
        if (rd < 0 || r1 < 0 || r2 < 0) { fprintf(stderr, "line %d: arithmetic requires registers\n", source_line); return -1; }
        out->op = base; out->a = rd; out->b = r1; out->c = r2; return 0;
    }

    if (base == OP_LOAD_RA) {
        if (ntok != 3) { fprintf(stderr, "line %d: LOAD Rdst, [addr|Rsrc]\n", source_line); return -1; }
        int rd = parse_reg(tok[1]); int is_reg = 0; int32_t val = 0;
        if (rd < 0 || parse_mem_operand(tok[2], &is_reg, &val) != 0) { fprintf(stderr, "line %d: invalid LOAD operands\n", source_line); return -1; }
        out->op = is_reg ? OP_LOAD_RR : OP_LOAD_RA; out->a = rd; out->b = val; out->imm = val; return 0;
    }

    if (base == OP_STORE_RA) {
        if (ntok != 3) { fprintf(stderr, "line %d: STORE Rsrc, [addr|Rdst]\n", source_line); return -1; }
        int rs = parse_reg(tok[1]); int is_reg = 0; int32_t val = 0;
        if (rs < 0 || parse_mem_operand(tok[2], &is_reg, &val) != 0) { fprintf(stderr, "line %d: invalid STORE operands\n", source_line); return -1; }
        out->op = is_reg ? OP_STORE_RR : OP_STORE_RA; out->a = rs; out->b = val; out->imm = val; return 0;
    }

    if (base == OP_CMP_RR) {
        if (ntok != 3) { fprintf(stderr, "line %d: CMP Rleft, Rright|imm\n", source_line); return -1; }
        int r1 = parse_reg(tok[1]); int r2 = parse_reg(tok[2]);
        if (r1 < 0) { fprintf(stderr, "line %d: invalid CMP left operand\n", source_line); return -1; }
        if (r2 >= 0) { out->op = OP_CMP_RR; out->a = r1; out->b = r2; return 0; }
        int32_t imm = 0;
        if (parse_i32(tok[2], &imm) != 0) { fprintf(stderr, "line %d: invalid CMP right operand\n", source_line); return -1; }
        out->op = OP_CMP_RI; out->a = r1; out->imm = imm; return 0;
    }

    if (base == OP_JMP || base == OP_JE || base == OP_JNE || base == OP_JG || base == OP_JL) {
        if (ntok != 2) { fprintf(stderr, "line %d: branch requires target\n", source_line); return -1; }
        int32_t target = 0;
        if (parse_i32(tok[1], &target) != 0 && find_label(st, tok[1], &target) != 0) {
            fprintf(stderr, "line %d: unknown label or address '%s'\n", source_line, tok[1]); return -1;
        }
        out->op = base; out->imm = target; return 0;
    }

    fprintf(stderr, "line %d: unsupported instruction\n", source_line);
    return -1;
}

int assemble_file(const char *input_path, const char *output_path) {
    SourceLine lines[MAX_LINES];
    int line_count = 0;
    if (collect_lines(input_path, lines, &line_count) != 0) return -1;
    SymbolTable st; memset(&st, 0, sizeof(st));
    if (first_pass(lines, line_count, &st) != 0) return -1;

    Instruction program[VM_MAX_PROGRAM];
    uint32_t count = 0;
    for (int i = 0; i < line_count; ++i) {
        if (count >= VM_MAX_PROGRAM) { fprintf(stderr, "program too large\n"); return -1; }
        char line[LINE_LEN]; strncpy(line, lines[i].text, sizeof(line) - 1); line[sizeof(line) - 1] = '\0';
        Instruction ins;
        int rc = encode_line(line, lines[i].source_line, &st, &ins);
        if (rc < 0) return -1;
        if (rc == 0) program[count++] = ins;
    }

    FILE *out = fopen(output_path, "wb");
    if (!out) { perror(output_path); return -1; }
    ProgramHeader header = { VM_MAGIC, VM_VERSION, count };
    if (fwrite(&header, sizeof(header), 1, out) != 1 || fwrite(program, sizeof(Instruction), count, out) != count) {
        perror("write bytecode"); fclose(out); return -1;
    }
    fclose(out);
    printf("assembled %s -> %s (%u instructions)\n", input_path, output_path, count);
    return 0;
}
