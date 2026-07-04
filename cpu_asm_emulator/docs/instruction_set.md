# MiniCPU Instruction Set

MiniCPU is a small educational register machine designed for CPU architecture, assembler, emulator, and compiler-backend experiments.

## Architecture

- 8 signed 32-bit general-purpose registers: `R0` ... `R7`
- 4096 signed 32-bit memory words
- program counter `PC`
- comparison flag `CMP`: -1, 0, +1
- deterministic execution with a max-step guard
- binary bytecode format with a versioned header

## Syntax

Comments start with `;` or `#`.
Labels end with `:`.

```asm
MOV R1, 10
loop:
SUB R1, R1, R2
CMP R1, 0
JG loop
HALT
```

## Instructions

| Instruction | Meaning |
|---|---|
| `NOP` | No operation |
| `MOV Rdst, imm` | Load immediate value |
| `MOV Rdst, Rsrc` | Register copy |
| `ADD Rdst, Ra, Rb` | Addition |
| `SUB Rdst, Ra, Rb` | Subtraction |
| `MUL Rdst, Ra, Rb` | Multiplication |
| `DIV Rdst, Ra, Rb` | Integer division |
| `LOAD Rdst, [addr]` | Load memory word |
| `LOAD Rdst, [Raddr]` | Load by register address |
| `STORE Rsrc, [addr]` | Store memory word |
| `STORE Rsrc, [Raddr]` | Store by register address |
| `CMP Ra, Rb` | Compare registers |
| `CMP Ra, imm` | Compare register and immediate |
| `JMP label` | Unconditional jump |
| `JE label` | Jump if equal |
| `JNE label` | Jump if not equal |
| `JG label` | Jump if greater |
| `JL label` | Jump if less |
| `PRINT Rn` | Print register |
| `HALT` | Stop execution |

## Practical Uses

MiniCPU can be used to study:

- instruction encoding
- assembly parsing
- label resolution
- bytecode execution
- debugging and tracing
- interpreter design
- compiler backend generation
