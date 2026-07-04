# low-level-systems-lab-portable-working

A practical low-level systems laboratory in C and assembly-oriented design.

This repository includes three integrated systems projects:

1. A custom mini CPU, assembler, disassembler, and emulator.
2. Portable C numerical kernels with an optional assembly dot-product path.
3. A tiny x86 bootloader operating system source tree with optional image generation.

The default validation path is designed to work on macOS and Linux with only a standard C compiler and `make`.
The bootable OS image target is optional because it requires x86 GNU binutils or a cross toolchain.

## Quick Start

```bash
cd ~/Downloads
unzip -o low-level-systems-lab-portable-working.zip
cd low-level-systems-lab-portable-working
./run.sh
```

Equivalent command:

```bash
make validate
```

Expected final output:

```text
all portable tests passed
full portable validation passed
```


## Quick Start on macOS

```bash
cd ~/Downloads
unzip -o low-level-systems-lab-macos-working.zip
cd low-level-systems-lab-macos-working
./run.sh
```

The archive does not include precompiled binaries. All executables are built locally on your machine to avoid architecture mismatch errors.

## Project Structure

```text
low-level-systems-lab-portable-working/
├── cpu_asm_emulator/
├── asm_matrix_kernel/
├── tiny_os/
├── docs/
├── .github/workflows/ci.yml
├── Makefile
├── run.sh
├── README.md
└── BUILD_VERIFICATION.md
```

## Mini CPU Assembler and Emulator

The mini CPU project implements:

- A custom instruction set
- Register file R0 to R7
- Program counter
- Comparison flag
- Word-addressable memory
- Assembler
- Disassembler
- Emulator
- Trace output
- Memory dump support
- Example assembly programs

Run it directly:

```bash
cd cpu_asm_emulator
make test
```

## Matrix Kernel

The matrix kernel project implements:

- Dot product
- AXPY
- Vector scale
- L2 norm
- Matrix-vector multiplication
- Matrix-matrix multiplication
- Benchmark program
- Unit tests
```
