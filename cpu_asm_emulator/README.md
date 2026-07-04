# mini-cpu-assembler-emulator

A practical educational CPU, assembler, disassembler, emulator, execution tracer, and memory inspection tool written in C.

This project demonstrates how assembly-like instructions are translated into bytecode and executed through registers, memory, a program counter, and comparison flags.

## Build

```bash
make
```

## Assemble and Run

```bash
./bin/minicpu asm programs/factorial.asm build/factorial.bin
./bin/minicpu run build/factorial.bin
```

Expected output:

```text
R1 = 120
```

## Trace Execution

```bash
./bin/minicpu run --trace --max-steps 1000 build/factorial.bin
```

## Disassemble Bytecode

```bash
./bin/minicpu disasm build/factorial.bin
```

## Dump Memory

```bash
./bin/minicpu asm programs/memory_sum.asm build/memory_sum.bin
./bin/minicpu run --dump-memory 200 5 build/memory_sum.bin
```

## Examples

```bash
make examples
```

## Research-Oriented Features

- deterministic execution with a max-step guard
- trace output for instruction-level analysis
- bytecode serialization
- disassembly for inspection
- memory dump support
- label resolution in the assembler
- register and immediate operand support
- direct and register-based memory addressing

## Why This Is Useful

- Understand registers, memory, instructions, and program counters
- Learn how assemblers resolve labels
- Learn the bridge between C, assembly, and CPU execution
- Prepare for OS, compiler, emulator, and embedded systems projects
- Provide a backend target for future compiler experiments
