# Validation Protocol

This document defines reproducible validation steps for the repository.

## Full Validation

Run:

```bash
make clean test
```

This should:

1. build and run MiniCPU examples
2. generate bytecode for sample programs
3. run the CPU emulator
4. generate disassembly and trace artifacts
5. build and test the matrix kernel
6. build the TinyOS disk image

## English-Only Check

Run:

```bash
find . -type f \
  \( -name "*.md" -o -name "*.c" -o -name "*.h" -o -name "*.txt" -o -name "*.yml" -o -name "*.S" -o -name "*.asm" -o -name "Makefile" \) \
  -not -path "./.git/*" -print0 \
| xargs -0 perl -CSDA -ne 'print "$ARGV:$.:$_" if /[\x{3040}-\x{30FF}\x{4E00}-\x{9FFF}]/'
```


## Matrix Kernel Validation

Run:

```bash
cd asm_matrix_kernel
make test
make bench
```

The C and assembly dot-product results should match within tolerance.

## TinyOS Validation

Run:

```bash
cd tiny_os
make image
```

The command should produce:

```text
dist/tinyos.img
```

If QEMU is available, run:

```bash
make run
```
