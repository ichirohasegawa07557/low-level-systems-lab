# Engineering Quality

This repository is designed to be clear, testable, and extendable.

## Quality Practices

- modular project layout
- Makefile-based builds
- unit-style test targets
- deterministic sample programs
- CI workflow
- English documentation
- validation protocol
- explicit limitations and extension paths

## Limitations

This project is not a production CPU, BLAS library, or operating system. It is a practical research and education platform. The goal is transparency and extensibility rather than maximum performance or complete hardware compatibility.

## Reliability Notes

- MiniCPU uses a max-step guard to prevent infinite loops.
- The assembler reports invalid opcodes and invalid operands.
- Matrix tests compare C and assembly results within numerical tolerance.
- TinyOS verifies that the bootloader is exactly 512 bytes.
